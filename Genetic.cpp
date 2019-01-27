#include "pch.h"
#include "Genetic.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
Genetic::Genetic()
{
}
Genetic::~Genetic()
{
}
int Genetic::GeneticTSP(int ileIteracji, int rozmiarPokolenia, double prawdopodobienstwoMutacji, bool wyswietlanie)
{
	//if (this == NULL) return;
	//std::cout<<"\nAlgorytm genetyczny:\n"<<endl;
	//zmienne pomocnicze
	int i, j, k, l, m, temp, max, losowanie1, losowanie2, licznik, rodzic1, rodzic2, ktoryRodzic, ileZwiekszyc, najlepszaSciezka, najkrotsza;
	int pierwszeMiasto, pierwszeWstawienie, miejsceOd1, miejsceOd2, miejsceDo1, miejsceDo2, miejsce, miasto1, miasto2;
	int najlepsza, droga_temp, droga;
	int *kolejnosc_temp = new int[getSize()];
	int *kolejnosc = NULL;
	int *tab_najlepsza = new int[getSize()];
	int *najlepszaGlobalnie = new int[getSize()];
	bool znaleziono1, znaleziono2;
	//koniec zmiennych pomocniczych

	int **nowePokolenie, **starePokolenie;
	int **pokolenie1, **pokolenie2;
	int *fitness = new int[rozmiarPokolenia];
	int *fitnessRank = new int[rozmiarPokolenia];
	pokolenie1 = new int*[rozmiarPokolenia];
	pokolenie2 = new int*[rozmiarPokolenia];
	int* sciezkaTemp = new int[getSize()];
	int* nowaSciezka = new int[getSize()];
	char znak;

	//tworzenie i inicjalizacja pierwszego pokolenia (losowo)
	max = (rozmiarPokolenia)*(rozmiarPokolenia + 1) / 2;
	nowePokolenie = NULL;
	starePokolenie = NULL;
	najkrotsza = INT_MAX;
	for (i = 0; i < rozmiarPokolenia; i++)
	{

		pokolenie1[i] = new int[this->getSize()];
		pokolenie2[i] = new int[this->getSize()];
		for (j = 0; j < this->getSize(); j++)
		{
			pokolenie1[i][j] = j;
		}

		for (j = getSize() - 1; j >= 0; j--)  //wymieszanie tablicy liczb
		{
			k = rand() % (j + 1);
			temp = pokolenie1[i][k];
			pokolenie1[i][k] = pokolenie1[i][j];
			pokolenie1[i][j] = temp;
		}

	}

	for (i = 0; i < ileIteracji; i++)
	{
		
		if (i % 2 == 0)
		{
			starePokolenie = pokolenie1;
			nowePokolenie = pokolenie2;
		}
		else
		{
			starePokolenie = pokolenie2;
			nowePokolenie = pokolenie1;
		}

		for (j = 0; j < rozmiarPokolenia; j++) //obliczenie drogi, zaincjalizowanie tablicy fitnessRank
		{
			fitness[j] = wylicz_droge(starePokolenie[j]);
			fitnessRank[j] = j;
		}
		//zmodyfikowany qsort dla tablic fitness i fitnessRank
		quicksortUp(fitness, fitnessRank, 0, rozmiarPokolenia - 1);
		if (fitness[0] < najkrotsza) //update najlepszej globalnie drogi
		{
			najkrotsza = fitness[0];
			for (j = 0; j < getSize(); j++)
			{
				najlepszaGlobalnie[j] = starePokolenie[0][j];
			}
		}

		if (wyswietlanie)
		{
			std::cout << "Iteracja " << i + 1 << std::endl << std::endl;
			std::cout << "Obecne pokolenie:\n";
			for (j = 0; j < rozmiarPokolenia; j++)
			{
				std::cout << "Osobnik " << j + 1 << ":\n";
				wyswietl_tab(starePokolenie[j], getSize());
				std::cout << "Dlugosc sciezki: " << fitness[j] << ":\n\n";
			}
		}
		if (wyswietlanie)
		{
			std::cout << "Najlepsza sciezka:" << std::endl;
			wyswietl_tab(najlepszaGlobalnie, getSize());
			std::cout << "Dlugosc: " << najkrotsza << std::endl << std::endl;
		}
		if (wyswietlanie) std::cout << "Generowanie nowego pokolenia: \n\n";


		for (j = 0; j < rozmiarPokolenia; j++)
		{
			//losowanie rodziców
			losowanie1 = rand() % max;
			losowanie2 = rand() % max;
			licznik = 0;
			ileZwiekszyc = rozmiarPokolenia;
			ktoryRodzic = 0;
			znaleziono1 = znaleziono2 = false;
			while (!(znaleziono1&&znaleziono2)) //wybór rodziców zgodnie z prawdopodobienstwem zaleznym od rankingu osobników
			{
				licznik += ileZwiekszyc;
				if (!znaleziono1)
				{
					if (licznik > losowanie1)
					{
						rodzic1 = fitnessRank[ktoryRodzic];
						znaleziono1 = true;
					}
				}

				if (!znaleziono2)
				{
					if (licznik > losowanie2)
					{
						rodzic2 = fitnessRank[ktoryRodzic];
						znaleziono2 = true;
					}
				}
				ktoryRodzic++;
				ileZwiekszyc--;
			}

		/*	if (wyswietlanie)
			{
				cout << "Rodzic 1" << endl;
				wyswietl_tab(starePokolenie[rodzic1], getSize());
				cout << "Rodzic 2" << endl;
				wyswietl_tab(starePokolenie[rodzic2], getSize());

			}*/

			pierwszeMiasto = rand() % getSize(); //wybierz poczatkowe miasto od którego bedziemy brac miasta od rodziców

			for (k = 0; k < getSize(); k++) //zaincjalizuj nowa sciezka ktora bedzie sciezka dziecka
			{
				nowaSciezka[k] = -1;
				sciezkaTemp[k] = k;
			}

			pierwszeWstawienie = getSize() % 2 == 0 ? getSize() / 2 : (getSize() - 1) / 2;
			nowaSciezka[pierwszeWstawienie] = pierwszeMiasto;
			sciezkaTemp[pierwszeMiasto] = INT_MIN;

			miejsceOd1, miejsceOd2, miejsceDo1, miejsceDo2;
			//znalezienie pozycji gdzie znajduje sie pierwsze wstawione miasto
			for (k = 0; k < getSize(); k++)
			{
				if (starePokolenie[rodzic1][k] == pierwszeMiasto)
				{
					miejsceOd1 = k;
					break;
				}
			}
			for (k = 0; k < getSize(); k++)
			{
				if (starePokolenie[rodzic2][k] == pierwszeMiasto)
				{
					miejsceOd2 = k;
					break;
				}
			}



			miejsceDo1 = pierwszeWstawienie + 1;
			miejsceDo2 = pierwszeWstawienie - 1;

			znaleziono1 = znaleziono2 = false;

			while (!(znaleziono1&&znaleziono2))
			{
				if (!znaleziono1)
				{
					miejsceOd1++;

					miejsceOd1 %= getSize();

					miasto1 = starePokolenie[rodzic1][miejsceOd1];
					for (l = 0; l < getSize(); l++)
					{
						if (nowaSciezka[l] == miasto1) //jeœli wczeœniej dodano to miasto to nie dodawaj i nie pobieraj wiecej z rodzica 1
						{
							znaleziono1 = true;
							break;
						}
					}
					if (!znaleziono1)
					{
						//cout << "miasto1 " << miasto1 << endl;
						sciezkaTemp[miasto1] -= 3 * getSize(); //zapamietanie które miasto zostalo juz dodane
						nowaSciezka[miejsceDo1] = miasto1; //zapisanie miasta w nowej sciezce
						miejsceDo1++;
						miejsceDo1 %= getSize();
					}
				}

				if (!znaleziono2)
				{

					miejsceOd2--;

					if (miejsceOd2 < 0) miejsceOd2 += getSize();

					miasto2 = starePokolenie[rodzic2][miejsceOd2];
					for (l = 0; l < getSize(); l++)
					{
						if (nowaSciezka[l] == miasto2) //jezli wczeœniej dodano to miasto to nie dodawaj i nie pobieraj wiecej z rodzica 2
						{
							znaleziono2 = true;
							break;
						}
					}
					if (!znaleziono2)
					{
						//cout << "miasto2 " << miasto2 << endl;

						sciezkaTemp[miasto2] -= 3 * getSize();//zapamietanie które miasto zostalo juz dodane
						nowaSciezka[miejsceDo2] = miasto2;//zapisanie miasta w nowej œcie¿ce
						miejsceDo2--;
						if (miejsceDo2 < 0) miejsceDo2 += getSize();
					}
				}
			}





			miejsce = miejsceDo1;
			for (k = getSize() - 1; k >= 0; k--)  //wymieszanie tablicy miast pozostalych do dodania
			{
				l = rand() % (k + 1);
				temp = sciezkaTemp[l];
				sciezkaTemp[l] = sciezkaTemp[k];
				sciezkaTemp[k] = temp;
			}

			for (k = 0; k < getSize(); k++)
			{
				if (sciezkaTemp[k] > -getSize()) //jezli miasto nie zostalo jeszcze dodane to dodaj
				{
					nowaSciezka[miejsce] = sciezkaTemp[k];
					miejsce++;
					miejsce %= getSize();
				}

			}

			for (k = 0; k < getSize(); k++)
			{
				nowePokolenie[j][k] = nowaSciezka[k]; //zapisz sciezke
			}

			if (wyswietlanie)
			{
				cout << "Dziecko " << j + 1 << endl;
				wyswietl_tab(nowePokolenie[j], getSize());
				std::cout << "Dlugosc: " << wylicz_droge(nowePokolenie[j]) << std::endl;

			}

			if ((double)rand() / RAND_MAX < prawdopodobienstwoMutacji) //jesli mutujemy to wykonaj zamiane 2-opt na nowo utworzonym dziecku
			{	
				kolejnosc = nowePokolenie[j];

				najlepsza = droga = wylicz_droge(kolejnosc);
				bool zmiana;
				do
				{
					zmiana = false;

					for (k = 0; k < getSize() - 1; k++)
					{
						for (int l = k + 1; l < getSize(); l++)
						{
							//dokonanie zamiany dwukrawedziowej i sprawdzenie dlugosci drogi po zamianie
							opt_sciezka(kolejnosc, kolejnosc_temp, k, l);

							droga_temp = wylicz_droge(kolejnosc_temp);

							if (droga_temp < najlepsza)
							{

								najlepsza = droga_temp;
								for (m = 0; m < getSize(); m++)
								{
									tab_najlepsza[m] = kolejnosc_temp[m];
								}
								zmiana = true;
							}

						}

					}
					if (zmiana)//ustaw najlepsza zamiana
					{
						for (k = 0; k < getSize(); k++)
						{
							kolejnosc[k] = tab_najlepsza[k];
						}
					}

				} while (zmiana);//wykonuj az jest poprawa

				if (wyswietlanie)
				{
					cout << "Dziecko " << j + 1 << " po mutacji" << endl;
					wyswietl_tab(nowePokolenie[j], getSize());
					std::cout << "Dlugosc: " << wylicz_droge(nowePokolenie[j])<< std::endl;

				}
			}
			if (wyswietlanie)std::cout << std::endl;
		}




	
	}
	if (wyswietlanie)
	{
		std::cout << "\nOsiagnieto warunek stopu, wyjscie z petli glownej\n";
	}

	for (i = 0; i < rozmiarPokolenia; i++)
	{
		fitness[i] = wylicz_droge(nowePokolenie[i]);
		if (fitness[i] < najkrotsza)
		{
			najkrotsza = fitness[i];
			for (j = 0; j < getSize(); j++)
			{
				najlepszaGlobalnie[j] = nowePokolenie[i][j];
			}
		}
	}

	//std::cout<<"Najlepsza sciezka:"<<std::endl;
	//wyswietl_tab(najlepszaGlobalnie, getSize());
	//std::cout<<"Dlugosc: "<<najkrotsza<<std::endl;


	return najkrotsza;
	delete[]kolejnosc_temp;
	delete[]najlepszaGlobalnie;
	delete[]fitness;
	delete[]fitnessRank;
	for (i = 0; i < rozmiarPokolenia; i++)
	{
		delete[]pokolenie1[i];
		delete[]pokolenie2[i];
	}
	delete[]pokolenie1;
	delete[]pokolenie2;
	delete[]tab_najlepsza;
	delete[]sciezkaTemp;
	delete[]nowaSciezka;

}

void Genetic::opt_sciezka(int* kolejnosc, int*wynik, int a, int b)
{
	//kolejnosc - droga która zmieniamy, a,b - wierzcholki które zamieniamy
	for (int i = 0; i < a; i++)
	{
		wynik[i] = kolejnosc[i];
	}
	for (int i = b + 1; i < getSize(); i++)
	{
		wynik[i] = kolejnosc[i];
	}
	//odwróc sciezke
	while (a <= b)
	{
		wynik[a] = kolejnosc[b];
		wynik[b] = kolejnosc[a];
		a++;
		b--;
	}
}

void Genetic::quicksortUp(int *priorytet, int *kolejnosc, int lewy, int prawy)
{
	double pivot = priorytet[((lewy + prawy) / 2)];
	int i = lewy;
	int j = prawy;
	do
	{
		while (priorytet[i] < pivot) i++;
		while (priorytet[j] > pivot) j--;
		if (i <= j)
		{
			double temp = priorytet[i];
			priorytet[i] = priorytet[j];
			priorytet[j] = temp;
			int temp2 = kolejnosc[i];
			kolejnosc[i] = kolejnosc[j];
			kolejnosc[j] = temp2;
			i++;
			j--;
		}
	} while (i <= j);
	if (j > lewy) quicksortUp(priorytet, kolejnosc, lewy, j);
	if (i < prawy) quicksortUp(priorytet, kolejnosc, i, prawy);
}

int Genetic::wylicz_droge(int*kolejnosc)
{
	int droga = 0;
	for (int i = 0; i < getSize() - 1; i++)
	{
		droga += table[kolejnosc[i]][kolejnosc[i + 1]];
	}
	droga += table[kolejnosc[getSize() - 1]][kolejnosc[0]];
	return droga;
}



void Genetic::wyswietl_tab(int*tab, int x)
{
	for (int i = 0; i < x; i++)
	{
		cout << tab[i] << " ";
	}
	cout << endl;
}