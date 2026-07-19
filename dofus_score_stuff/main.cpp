
#include "stuff.h" 

//double MALUS_SCORE_PM{ 0 };
//int NB_PA_VISE{ 3 };

std::random_device RANDOM_SEED;
std::mt19937 RAND_GENERATOR(RANDOM_SEED());
std::uniform_real_distribution<> RAND_01(0.0, 1.0);

vector<double> SCORES{}; // à remplir avant la recherche de stuff !
vector<str> NOMS_CARAC{};
vector<bool> CARAC_SPE{};

int PA_BASE{ 7 };

//const double SCORE_STATS{ 7 }, SCORE_DOMMAGES{ 23.33 };
// un score à -1 signifie que la stat est speciale et doit etre regardée en detail 
//double SCORE_VIE{1}, SCORE_PUI{SCORE_STATS}, SCORE_FORCE{0}, SCORE_INTEL{0}, SCORE_AGI{SCORE_STATS}, SCORE_CHANCE{0}, SCORE_DO{SCORE_DOMMAGES}, SCORE_DO_NEUTRE{0}, SCORE_DO_T{0}, SCORE_DO_F{0}, SCORE_DO_A{SCORE_DOMMAGES}, SCORE_DO_E{0}, SCORE_PA{-1}, SCORE_PM{116.66}, SCORE_PO{23.33}, SCORE_INVO{0};

int main()
{
	const str FICHIER_ITEMS{ "items.txt" }, FICHIER_PANOS{ "panos.txt" }, FICHIER_STUFF{ "stuff.txt" }, FICHIER_SCORES{ "scores_stats.txt" }, FICHIER_NOUVEAUX_ITEMS{ "items_dofus_book.txt" }, FICHIER_NOUVELLES_PANOS{ "panos_dofus_book.txt" };
	// ne pas hesiter a trouver dofus-stuff.ods pour editer les listes de texte 

	{
		str txt{ "" };
		txt += "\n\nBienvenue dans l'optimisateur de stuff Dofus !\n\n";
		txt += "Les explication d'utilisation se trouvent dans \"Informations.txt\"\n\n";
		txt += "Fichier a remplir pour que le programme fonctionne :\n";
		txt += "- " + FICHIER_ITEMS + " doit contenir la liste des items a utiliser\n";
		txt += "- " + FICHIER_PANOS + " doit contenir les bonus des panoplies \n";
		txt += "- " + FICHIER_SCORES + " doit contenir les scores associes a chaque satistique\n";
		txt += "\n";
		txt += "Autres fichiers pouvant etre utiles :\n";
		txt += "- " + FICHIER_NOUVEAUX_ITEMS + " avec les items de dofus book a formater pour le programme\n";
		txt += "- " + FICHIER_NOUVELLES_PANOS + " avec les panoplies de dofus book a formater pour le programme\n";
		txt += "\n";
		std::cout << txt;
	}


	

	int a{ 0 };
	a = GetIntSec("Entrer un choix puis faire Entrer :\n0 : optimiser le stuff\n1 : formater des equipements\n2 : formater des panoplies\n3 : formater des equipement et des panoplies\n", 0, 3);

	if (a == 1 || a == 3)
	{
		//CreePanos("panos_30_130.txt");
		CreeItems(FICHIER_NOUVEAUX_ITEMS);
	}
	if (a == 2 || a == 3)
	{
		CreePanos(FICHIER_NOUVELLES_PANOS);
	}

	if (a != 0)
	{
		Pause("Fin du programme\n");
		return 0;
	}

	if (FichierLisible(FICHIER_SCORES))
	{
		str l{ "" };
		std::ifstream s{ FICHIER_SCORES };
		//std::getline(s, l);

		std::getline(s, l);
		const vector<str> noms{ Mots(l, '	', false) };
		std::getline(s, l);
		const vector<str> scores{ Mots(l, '	', false) };

		NOMS_CARAC = vector<str>(NB_CARAC, "");
		SCORES = vector<double>(NB_CARAC, 0);
		CARAC_SPE = vector<bool>(NB_CARAC, false);
		for (int i = 0; i < NB_CARAC; i++)
		{
			NOMS_CARAC[i] = noms[i];
			SCORES[i] = StrToDouble(scores[i]);
		}
		CARAC_SPE[PA] = true;
		SCORES[PA] = 0; // le score des caracs spe doit etre 0 
	}
	else
	{
		std::cout << "le fichier de score doit se trouver dans le meme dossier, nomme : \"" << FICHIER_SCORES << "\"\n";
		std::cout << "le programme ne peut pas fonctionner san ce fichier ...\n";
		Pause("Le programme est termine\n");
		return 1;
	}

	{
		std::cout << "\nScore des stats : \n";
		for (int i = 0; i < NB_CARAC; i++)
		{
			if (SCORES[i] > 0)
			{
				std::cout << NOMS_CARAC[i] << " : " << SCORES[i] << "\n";
			}
			else if (CARAC_SPE[i])
			{
				std::cout << NOMS_CARAC[i] << " : special\n";
			}
		}
		std::cout << "\n";
	}
	
	vector<vector<Citem>> items_type{ vector<vector<Citem>>(FIN_TYPE, vector<Citem>{}) };
	for (int i = 0; i < T(items_type); i++)
	{
		items_type[i].push_back(Citem("aucun", i, Cbonus(0), 0));
	}
	//items_type[ANNEAU].push_back(Citem("aucun", ANNEAU, Cbonus(0), 0));

	// recuperation de tous les items 
	{
		std::ifstream s{ FICHIER_ITEMS };
		//std::cout << FichierLisible(FICHIER_ITEMS) << "\n";
		str l{ "" };
		vector<str> mots{};
		int type{ 0 };

		while (std::getline(s, l)) // >> std::ws efface les tab en debut de ligne 
		{
			//std::cout << l << "\n";
			mots = Mots(l);
			//std::cout << T(mots) << "\n";
			if (T(mots) > NB_MOTS)
			{
				type = StrToType(mots[2]);
				//std::cout << type << " " << mots[2] << " " << mots[1] << "\n";
				const Citem it(mots[1], type, Cbonus(mots), StrToInt(mots[3]), mots[0]);
				//std::cout << it.Gscore() << " " << mots[11] << "\n";
				items_type[type].push_back(it);
				if (type == ANNEAU && mots[0] == "")
				{
					//items_type[type].push_back(it); // si on fait Clear_1 les anneaux sont mis apres 
				}
				//std::cout << it.Gprecis() << "\n";
			}
		}
	}
	AffItems(items_type, "items recuperes : \n");

	// recuperation des panos 
	vector<Cpano> panos{};
	{
		std::ifstream s{ FICHIER_PANOS };
		str l{ "" };
		vector<str> mots{};

		while (std::getline(s >> std::ws, l))
		{
			//std::cout << l << "\n";
			mots = Mots(l);
			if (T(mots) > NB_MOTS)
			{
				bool aj{ false }; // si la pano a ete ajoutee par le passe 
				for (int i = 0; i < T(panos); i++)
				{
					if (mots[0] == panos[i].Gnom())
					{
						panos[i].Aj(Cbonus(mots), StrToInt(mots[1]));
						aj = true;
						break;
					}
				}
				if (!aj)
				{
					Cpano n(mots[0]);
					n.Aj(Cbonus(mots), StrToInt(mots[1]));
					panos.push_back(n);
				}
			}
		}
	}
	for (int i = 0; i < T(panos); i++)
	{
		//panos[i].Verifie(); // il y a qq panos qui donnent des malus, mais à priori pas assez pour etre mauvaises, à voir, au pire il faudra enlever les items un par un apres test 
	}
	std::cout << "panos recuperees : " << T(panos) << " panos\n\n";
	//std::cout << panos[0].Gprecis() << "\n";

	PA_BASE = GetInt("Nombre de PA sans equipement : ");
	std::cout << "\n";

	if (GetIntSec("0 pour enlever les items inutiles, 1 sinon : ", 0, 1) == 0)
	{
		int niv_max{ GetInt("niveau max items : ") }; // GetInt("niveau max items : ")
		ClearItemsNiv(items_type, niv_max);


		ExporteItems("items_niv_" + std::to_string(niv_max) + ".txt", items_type);

		//AffItems(items_type, "il reste :\n");
		//DebugChercheItem("Gruchape", items_type);

		bool aff_clear{ !true };
		bool clear{ true }; // au moins un item a ete clear 
		//Clear_1(items_type, panos, aff_clear);
		while (clear)
		{
			clear = false;
			if (Clear_1(items_type, panos, aff_clear))
			{
				clear = true;
			}
			else if (ClearPano(items_type, panos, aff_clear))
			{
				clear = true;
			}
			else if (Clear_2(items_type, panos, aff_clear))
			{
				clear = true;
			}
			else if (Clear_3(items_type, panos, aff_clear))
			{
				clear = true;
			}
			else if (Clear_4(items_type, panos, aff_clear))
			{
				clear = true;
			}
			//DebugChercheItem("Gruchape", items_type);
		}
		AjAnneaux(items_type); // le fait qu'on fasse ca à la fin peut peut etre empecher qq suppressions avant ?? 
		ClearFinal(items_type, panos, aff_clear);

		if (!aff_clear)
		{
			AffItems(items_type, "\nfin de clear, items restants : \n");
		}


		ExporteItems("items_apres_clear.txt", items_type);
		Pause("Entrer pour passer a la recherche de stuff avec ces items\n", "");
	}


	// recherche les stuffs en fonction du nb de pa et du bonus de pano 
	const int mini{ GetInt("Total de PA minimum voulu : ") };
	const int maxi{ GetInt("Total de PA maximum voulu : ") };
	RechercheStuffPA(items_type, panos, mini, maxi);
	Pause("Fin du programme\n");
	return 0;
}