#pragma once

#include <vector> 
#include <array> 
#include <string> 
#include <fstream> 
#include <iostream> 
#include <algorithm> 
#include <random> 
#include <time.h> 
#include <numeric> 
#include <conio.h> // _getch

using std::vector;
using std::array;
#define str std::string 
#define ls vector<std::string> 

template<typename sizeable>
inline int T(sizeable const& liste)
{
	return int(std::size(liste));
}

template<typename a>
inline void MultiPushBack(std::vector<a>& liste, std::vector<a> const& liste_a_ajouter)
{
	for (int i = 0; i < T(liste_a_ajouter); i++)
	{
		liste.push_back(liste_a_ajouter[i]);
	}
}

template<typename a>
inline void MultiPushBackItem(vector<a>& liste, a item, int nb_ajoute)
{
	for (int i = 0; i < nb_ajoute; i++)
	{
		liste.push_back(item);
	}
}

template<typename a>
inline void Echange(std::vector<a>& vect, int pos1, int pos2)
{
	std::swap(vect[pos1], vect[pos2]); // tres legerement plus rapide 
}

// ne garde pas l'ordre de la liste !
template<typename a>
inline void Supp(std::vector<a>& vect, int pos_elt)
{
	Echange(vect, pos_elt, T(vect) - 1);
	vect.pop_back();
}

// garde l'ordre (et en plus ca a l'air plus rapide mdr)
template<typename popable>
inline void Supp_(popable& liste, int pos_elt)
{
	while (pos_elt < T(liste) - 1)
	{
		liste[pos_elt] = liste[pos_elt + 1];
		pos_elt++;
	}
	liste.pop_back();
}



template<typename elt>
bool Parmi(elt const& element, std::vector<elt> const& contenant)
{
	return std::any_of(contenant.cbegin(), contenant.cend(), [element](elt const& e) { return e == element; });
}

template<typename T>
bool ParmiPos(T const& elt, std::vector<T> const& liste, int& pos)
{
	const auto p{ std::find(liste.begin(), liste.end(), elt) };
	if (p != std::end(liste))
	{
		pos = (int(p - std::begin(liste)));
		return true;
	}
	return false;
}

// au moins un element de cherche est dans liste 
template<typename elt>
bool LunParmi(vector<elt> const& liste, vector<elt> const& cherche)
{
	for (int i = 0; i < T(cherche); i++)
	{
		if (Parmi(cherche[i], liste))
		{
			return true;
		}
	}
	return false;
}

// supprime une occurence si l'element est present dans liste, sans garder l'ordre de la liste 
template<typename elt>
void Del(std::vector<elt>& liste, elt const& element)
{
	int pos{ 0 };
	if (ParmiPos(element, liste, pos))
	{
		Supp(liste, pos);
	}
}

// met l'élément en premier, sans changer le reste de l'ordre 
template<typename elt>
void MetPremier(std::vector<elt>& l, elt const& element)
{
	for (int i = 0; i < T(l) - 1; i++)
	{
		if (l[0] == element)
		{
			return;
		}
		Echange(l, 0, i + 1);
	}
}


#undef max 

// vecteur non vide 
template<typename nb>
nb Max(std::vector<nb> const& l)
{
	return std::max(l);
}

// vecteur non vide 
template<typename nb>
int PosMax(std::vector<nb> const& l)
{
	int r{ 0 };
	nb m{ l[0] };
	for (int i = 1; i < T(l); i++)
	{
		if (l[i] > m)
		{
			m = l[i];
			r = i;
		}
	}
	return r;
}

int Somme(vector<int> const& liste)
{
	return std::accumulate(liste.begin(), liste.end(), 0);
}


constexpr int MAX_IGNORE{ 255 };
const std::string GEN_VAL_DEFAUT{ "Valeur par defaut" };

int GetInt(std::string const& message, std::string const& error = GEN_VAL_DEFAUT)
{
	int choice{ -1 };
	std::cout << message;
	while (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(MAX_IGNORE, '\n');
		if (error == GEN_VAL_DEFAUT)
		{
			std::cout << message;
		}
		else
		{
			std::cout << error;
		}
	}
	std::cin.clear();
	std::cin.ignore(MAX_IGNORE, '\n');
	return choice;
}

int GetIntSec(std::string const& message, int minimum, int maximum, std::string const& error = GEN_VAL_DEFAUT)
{
	int a{ GetInt(message, error) };
	while ((a > maximum) or (a < minimum))
	{
		if (error == GEN_VAL_DEFAUT)
		{
			a = GetInt(message, error);
		}
		else
		{
			a = GetInt(error, error);
		}
	}
	return a;
}

str GetStrSec(str const& message, str const& error = GEN_VAL_DEFAUT)
{
	std::string choice{ "" };
	std::cout << message;
	while (!(std::getline(std::cin, choice)))
	{
		std::cin.clear();
		std::cin.ignore(MAX_IGNORE, '\n');
		if (error == GEN_VAL_DEFAUT)
		{
			std::cout << message;
		}
		else
		{
			std::cout << error;
		}
	}
	std::cin.clear();
	std::cin.ignore(MAX_IGNORE, '\n');
	return choice;
}

str GetStr(str const& message)
{
	std::string choice{ "" };
	std::cout << message;
	std::getline(std::cin, choice);
	std::cin.clear();
	//std::cin.ignore(MAX_IGNORE, '\n');
	return choice;
}

// 13 : entree 
// 8 : efface 
char GetCharTpsReel()
{
	const char a{ (char)_getch() };

	if (a == -123)
	{
		return 'à';
	}
	if (a == -125)
	{
		return 'â';
	}
	if (a == -126)
	{
		return 'é';
	}
	if (a == -118)
	{
		return 'è';
	}
	if (a == -120)
	{
		return 'ê';
	}
	if (a == -116)
	{
		return 'î';
	}
	if (a == -109)
	{
		return 'ô';
	}
	if (a == -105)
	{
		return 'ù';
	}
	if (a == -106)
	{
		return 'û';
	}

	if (a == -121)
	{
		return 'ç';
	}

	return a;
}


inline int NbMs(clock_t const& tps)
{
	return (int)tps;
}

inline double NbS(clock_t const& tps)
{
	return ((float)tps) / CLOCKS_PER_SEC;
}

str HMS_EtReset(clock_t& tps)
{
	tps = clock() - tps;
	double s{ NbS(tps) };
	int m{ (int(s)) / 60 }, h{ m / 60 };
	str r{ "" };
	s -= m * 60;
	m = m % 60;
	if (h > 0)
	{
		r += std::to_string(h) + "h ";
	}
	if (m > 0)
	{
		r += std::to_string(m) + "min ";
	}
	r += std::to_string(s) + "s";
	tps = clock();
	return r;
}

inline void AfficheEtResetTps(clock_t& tps, std::string const& message = "temps ecoule : ", bool fin_ligne = true)
{
	tps = clock() - tps;
	std::cout << message;
	double s{ NbS(tps) };
	int m{ (int(s)) / 60 }, h{ m / 60 };
	s -= m * 60;
	m = m % 60;
	if (h > 0)
	{
		std::cout << h << "h ";
	}
	if (m > 0)
	{
		std::cout << m << "min ";
	}
	std::cout << s << "s";
	if (fin_ligne)
	{
		std::cout << "\n";
	}
	tps = clock();
}

inline str MinS(int ms)
{
	return std::to_string((ms / 1000) / 60) + "min " + std::to_string((ms / 1000) % 60) + "s";
}



inline bool FichierLisible(std::string const& nom_fichier)
{
	std::ifstream s{ nom_fichier };
	return s.is_open();
}


template<typename elt>
std::ofstream& operator<<(std::ofstream& stream, std::vector<elt> const& liste)
{
	const int t{ (int)std::size(liste) };
	stream << t << '\n';
	int i{ 0 };
	for (i = 0; i < t; i++)
	{
		stream << liste[i] << "\n";
	}
	return stream;
}

template<typename elt>
std::ifstream& operator>>(std::ifstream& stream, std::vector<elt>& liste)
{
	liste = {};
	int n{ 0 };
	elt a;
	stream >> n;
	for (int i = 0; i < n; i++)
	{
		stream >> a;
		liste.push_back(a);
	}
	//std::cout << "vector recup : " << Str(liste_positive) << "\n";
	return stream;
}


inline int Val(bool a)
{
	if (a)
	{
		return 1;
	}
	return 0;
}

inline bool Val(int a)
{
	if (a > 0)
	{
		return true;
	}
	return false;
}



// declaration dans main 
extern std::mt19937 RAND_GENERATOR;
extern std::uniform_real_distribution<> RAND_01;

inline double Rand()
{
	return RAND_01(RAND_GENERATOR);
}

inline int RandAB(int a, int b_non_inclu)
{
	return a + ((int)(Rand() * (b_non_inclu - a)));
}

// pas ouf 
inline int RandGrd(int nb_chiffres_max)
{
	int r{ 0 };
	for (int i = 0; i < nb_chiffres_max; i++)
	{
		r = r * 10 + RandAB(0, 10);
	}
	return r;
}

// liste non vide 
template<typename elt>
elt RandElt(std::vector<elt> const& liste)
{
	return liste[RandAB(0, T(liste))];
}




inline void Pause(std::string const& message = "", std::string const& message_2 = "Entrer pour continuer")
{
	std::cout << message << message_2;
	std::getchar();
}




int Compte(vector<int> const& liste, int cherche)
{
	return std::count(liste.begin(), liste.end(), cherche);
}


// je met ces trucs en double pour eviter l'overflow 
double Fact(int n)
{
	if (n < 2)
	{
		return 1;
	}
	return n * Fact(n - 1);
}

// verifier que ya pas d'overflow 
double KParmiN(int k, int n)
{
	return Fact(n) / (Fact(k) * Fact(n - k));
}


// on va retirer les accents du document texte depuis std::getline 
void RemplaceAccents(str& l)
{
	// depuis std::getline certains caracteres accentues sont dupliques en plusieurs char 
	// à -> -61 et -96
	// â -> -61 et -94
	// Â -> -61 et -126 
	// 
	// é -> -61 et -87
	// è -> -61 et -88
	// ê -> -61 et -86
	// ë -> -61 et -85
	// É -> -61 et -119
	// 
	// î -> -61 et -82
	// ï -> -61 et -81
	// 
	// ô -> -61 et -76
	// o -> -59 et -115 
	//
	// ü -> -61 et -68 
	// û -> -61 et -69
	//
	// ç -> -61 et -89 
	//
	// oe -> -59 et -109 
	// Oe -> -59 et -110 
	//
	// ae -> -61 et -90 
	int t{ T(l) };
	for (int i = 0; i < t; i++)
	{
		const bool r{ i < t - 1 }; // si il reste un charactere apres 
		if (l[i] == -61 && r && Parmi(l[i + 1], { -94, -96 }))
		{
			Supp_(l, i);
			t--;
			l[i] = 'a';
		}
		else if (l[i] == -61 && r && Parmi(l[i + 1], { -126 }))
		{
			Supp_(l, i);
			t--;
			l[i] = 'A';
		}
		else if (l[i] == -61 && r && Parmi(l[i + 1], { -87, -88, -86, -85 }))
		{
			Supp_(l, i);
			t--;
			l[i] = 'e';
		}
		else if (l[i] == -61 && r && Parmi(l[i + 1], { -119 }))
		{
			Supp_(l, i);
			t--;
			l[i] = 'E';
		}
		else if (l[i] == -61 && r && Parmi(l[i + 1], { -82, -81 }))
		{
			Supp_(l, i);
			t--;
			l[i] = 'i';
		}
		else if (r && ((l[i] == -61 && Parmi(l[i + 1], { -76 })) || (l[i] == -59 && l[i + 1] == -115)))
		{
			Supp_(l, i);
			t--;
			l[i] = 'o';
		}
		else if (l[i] == -61 && r && Parmi(l[i + 1], { -68, -69 }))
		{
			Supp_(l, i);
			t--;
			l[i] = 'u';
		}
		else if (l[i] == -61 && r && Parmi(l[i + 1], { -89 }))
		{
			Supp_(l, i);
			t--;
			l[i] = 'c';
		}
		else if (l[i] == -59 && r && Parmi(l[i + 1], { -109 }))
		{
			l[i] = 'o';
			l[i + 1] = 'e';
		}
		else if (l[i] == -59 && r && Parmi(l[i + 1], { -110 }))
		{
			l[i] = 'O';
			l[i + 1] = 'e';
		}
		else if (l[i] == -61 && r && Parmi(l[i + 1], { -90 }))
		{
			l[i] = 'a';
			l[i + 1] = 'e';
		}
		else if (l[i] < 0)
		{
			std::cout << l << " a un accent en pos " << i << "\n";
			std::cout << (int)l[i] << "\n";
			Pause();
		}
	}
}

// les mots doivent etres separes par un seprateur (de base une tabulation par exemple) 
// garde_mot_vide à false permet de separer plusieurs fois deux mots 
vector<str> Mots(str const& ligne, char sep = '	', bool garde_mot_vide = true)
{
	str mot{ "" };
	vector<str> r{};
	for (int i = 0; i < T(ligne); i++)
	{
		if (ligne[i] == sep)
		{
			if (garde_mot_vide || mot != "")
			{
				r.push_back(mot);
				mot = "";
			}
		}
		else
		{
			mot += ligne[i];
		}
	}
	if (mot != "")
	{
		r.push_back(mot);
	}
	return r;
}

str Phrase(vector<str> const& mots, str const& separateur = " ")
{
	str r{ "" };
	for (int i = 0; i < T(mots) - 1; i++)
	{
		r += mots[i] + separateur;
	}
	r += mots[T(mots) - 1];
	return r;
}

// donne 0 pour une chaine vide 
int StrToInt(str const& nb)
{
	if (std::empty(nb))
	{
		return 0;
	}
	double r{ 0 };
	int p{ 0 };
	const int t{ T(nb) };
	bool neg{ false };
	if (nb[0] == '-')
	{
		neg = true;
		p++;
	}
	while (p < t)
	{
		r *= 10;
		r += (nb[p] - '0');
		p++;
	}
	if (neg)
	{
		return -r;
	}
	return r;
}

bool StrToInt(str const& mot, int& nb)
{
	if (std::empty(mot))
	{
		return false;
	}
	double r{ 0 };
	int p{ 0 };
	const int t{ T(mot) };
	bool neg{ false };
	if (mot[0] == '-')
	{
		neg = true;
		p++;
	}
	while (p < t)
	{
		if ('0' > mot[p] or mot[p] > '9')
		{
			return false;
		}
		r *= 10;
		r += (mot[p] - '0');
		p++;
	}
	if (neg)
	{
		r = -r;
	}
	nb = r;
	return true;
}

double StrToDouble(str const& nb)
{
	if (std::empty(nb))
	{
		return 0;
	}
	double r{ 0 }, deci{ 0.1 };
	int p{ 0 };
	const int t{ T(nb) };
	bool neg{ false };
	if (nb[0] == '-')
	{
		neg = true;
		p++;
	}
	while (p < t)
	{
		if (nb[p] == '.' || nb[p] == ',')
		{
			p++;
			break;
		}
		r *= 10;
		r += (nb[p] - '0');
		p++;
	}
	while (p < t)
	{
		r += (nb[p] - '0') * deci;
		deci /= 10;
		p++;
	}
	if (neg)
	{
		return -r;
	}
	return r;
}



// ajoute (val * score_valeur) à somme si score_valeur n'est pas -1
void AddMultSi(double& somme, int val, double score_valeur)
{
	if (score_valeur != -1)
	{
		somme += val * score_valeur;
	}
}

void AffScore(str const& nom_stat, double score)
{
	if (score == 0)
	{
		return;
	}
	std::cout << nom_stat << " : ";
	if (score == -1)
	{
		std::cout << "special";
	}
	else
	{
		std::cout << score;
	}
	std::cout << "\n";
}



// liste non vide 
int Mediane(vector<int> l)
{
	std::sort(l.begin(), l.end());
	return l[T(l) / 2];
}


// les deux liste doivent avoir la meme taille 
double ProdScal(vector<int> const& a, vector<double> const& b)
{
	double r{ 0 };
	for (int i = 0; i < T(a); i++)
	{
		r += a[i] * b[i];
	}
	return r;
}

// les deux liste doivent avoir la meme taille 
void AjVect(vector<int>& a, vector<int> const& ajoute)
{
	for (int i = 0; i < T(a); i++)
	{
		a[i] += ajoute[i];
	}
}

vector<int> AddVect(vector<int> const& a, vector<int> const& b)
{
	vector<int> r{ a };
	AjVect(r, b);
	return r;
}

void RetVect(vector<int>& a, vector<int> const& retire)
{
	for (int i = 0; i < T(a); i++)
	{
		a[i] -= retire[i];
	}
}

vector<int> SoustrVect(vector<int> const& a, vector<int> const& b)
{
	vector<int> r{ a };
	RetVect(r, b);
	return r;
}
