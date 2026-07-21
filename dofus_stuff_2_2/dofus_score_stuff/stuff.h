#pragma once

#include "item.h" 

class Cstuff
{
public:
	Cstuff() : m_items({})
	{

	}

	Cstuff(Citem const& item) : m_items({ item })
	{

	}

	Cstuff(vector<Citem> const& items) : m_items(items)
	{

	}


	bool Correct() const
	{
		vector<int> compte{ vector<int>(FIN_TYPE, 0) };
		for (int i = 0; i < T(m_items); i++)
		{
			compte[m_items[i].Gtype()]++;
			if (compte[m_items[i].Gtype()] > 1)
			{
				if (m_items[i].Gtype() != ANNEAU)
				{
					return false;
				}
				else if (compte[m_items[i].Gtype()] > 2)
				{
					return false;
				}
			}
		}
		return true;
	}

	int NbItemsPano(str const& pano) const 
	{
		int r{ 0 };
		for (int i = 0; i < T(m_items); i++)
		{
			if (m_items[i].EstDePano(pano))
			{
				r++;
			}
		}
		return r;
	}

	Cbonus Gbonus_hors_pano() const
	{
		Cbonus r;
		for (int i = 0; i < T(m_items); i++)
		{
			r += m_items[i];
		}
		return r;
	}

	Cbonus Gbonus_juste_pano(vector<Cpano> const& panos) const
	{
		Cbonus r;
		//vector<str> panos_faites{};
		//str pano{ "" };
		//for (int i = 0; i < T(m_items); i++)
		//{
		//	pano = m_items[i].Gpano();
		//	if (pano == "" || Parmi(pano, panos_faites))
		//	{
		//		continue;
		//	}
		//	panos_faites.push_back(pano);
		//	for (int j = 0; j < T(panos); j++)
		//	{
		//		if (panos[j].Gnom() == pano)
		//		{
		//			r += panos[j].Gbonus(this->NbItemsPano(pano));
//
		//			break;
		//		}
		//	}
		//}
		//return r;
		for (int i = 0; i < T(panos); i++)
		{
			r += panos[i].Gbonus(this->NbItemsPano(panos[i].Gnom()));
		}
		return r;
	}

	Cbonus Gbonus(vector<Cpano> const& panos) const
	{
		{
			Cbonus r;
			for (int i = 0; i < T(m_items); i++)
			{
				r += m_items[i];
			}
			for (int i = 0; i < T(panos); i++) // wtf c'est plus rapide comme ca alors que ca semble faire plus d'operations !!
			{
				r += panos[i].Gbonus(this->NbItemsPano(panos[i].Gnom()));
			}
			return r;
		}

		Cbonus r;
		vector<str> panos_faites{};
		str pano{ "" };
		for (int i = 0; i < T(m_items); i++)
		{
			r += m_items[i];

			pano = m_items[i].Gpano();
			if (pano == "" || Parmi(pano, panos_faites))
			{
				continue;
			}
			panos_faites.push_back(pano);
			for (int j = 0; j < T(panos); j++)
			{
				if (panos[j].Gnom() == pano)
				{
					r += panos[j].Gbonus(this->NbItemsPano(pano));

					break;
				}
			}
		}
		return r;

		return this->Gbonus_hors_pano() + this->Gbonus_juste_pano(panos);
	}

	Cbonus Gbonus(vector<Cpano> const& panos, int& nb_bonus_pano) const
	{
		{
			Cbonus r;
			nb_bonus_pano = 0;
			int nb{ 0 };
			for (int i = 0; i < T(m_items); i++)
			{
				r += m_items[i];
			}
			for (int i = 0; i < T(panos); i++) // wtf c'est plus rapide comme ca alors que ca semble faire plus d'operations !!
			{
				nb = this->NbItemsPano(panos[i].Gnom());
				r += panos[i].Gbonus(nb);
				if (nb > 1)
				{
					nb_bonus_pano += (nb - 1);
				}
			}
			return r;
		}
	}

	// hors pano 
	double Gscore_hors_pano() const
	{
		return this->Gbonus_hors_pano().Gscore();
	}

	// juste pano 
	double Gscore_pano(vector<Cpano> const& panos) const
	{
		return this->Gbonus_juste_pano(panos).Gscore();
	}

	str G() const
	{
		str r{ "" };
		for (int i = 0; i < T(m_items); i++)
		{
			r += m_items[i].G() + "\n";
		}
		return r;
	}

	str Gprecis(vector<Cpano> const& panos) const
	{
		str bonus_txt{ "" };
		for (int i = 0; i < NB_CARAC; i++)
		{
			bonus_txt += NOMS_CARAC[i] + "	";
		}
		bonus_txt += "Explic Autres	Max PA	Max PM";
		str r{ "equipements\npano	nom	type	niv	" + bonus_txt + "\n" };
		Cbonus tot;
		int niv{ -1 };
		for (int i = 0; i < T(m_items); i++)
		{
			r += m_items[i].Gprecis() + "\n";
			tot += m_items[i];
			niv = std::max(niv, m_items[i].Gniv());
		}
		r += "\n\npano	nb items			" + bonus_txt + "\n";
		int saut{ 10 };
		for (int i = 0; i < T(panos); i++)
		{
			const int nb_items{ this->NbItemsPano(panos[i].Gnom()) };
			if (nb_items > 0)
			{
				r += panos[i].Gligne(nb_items);
				tot += panos[i].Gbonus(nb_items);
				saut--;
			}
		}
		for (int i = 0; i < saut; i++)
		{
			r += "\n";
		}
		r += "total				" + bonus_txt + "\n";
		r += "			" + std::to_string(niv) + "	" + tot.Gtexte_valeurs();
		return r;
	}

	int Gniv() const
	{
		int niv{ -1 };
		for (int i = 0; i < T(m_items); i++)
		{
			niv = std::max(niv, m_items[i].Gniv());
		}
		return niv;
	}


	int Gnb_bonus_pano() const
	{
		int r{ 0 };
		vector<str> panos{};
		for (int i = 0; i < T(m_items); i++)
		{
			const str pano{ m_items[i].Gpano() };
			if (Parmi(pano, panos))
			{
				r++;
			}
			else if (pano != "")
			{
				panos.push_back(pano);
			}
		}
		return r;
	}

	int Gnb_items() const
	{
		return T(m_items);
	}

	bool Ga_pano() const
	{
		for (int i = 0; i < T(m_items); i++)
		{
			if (m_items[i].Gpano() != "")
			{
				return true;
			}
		}
		return false;
	}


private:
	vector<Citem> m_items;
};





// renvoie la liste des stuff optimises avec ces items : 
// { {stuff sans pano}, {stuffs avec pano} }
// les stuffs sont tries par valeur de pa, du minimum au maximum 
// ainsi r[1][4] est le meilleur stuff avec pano de (4 + pa_min) PA 
vector<vector<Cstuff>> RechercheStuffPA(vector<vector<Citem>> const& items_type, vector<Cpano> const& panos, int pa_min, int pa_max)
{
	vector<vector<Cstuff>> r{ {}, {} }; // r[0] pour sans pano, et r[1] avec pano ; r[x] est le vecteur des stuff range dans l'ordre de PA : r[x][0] donne pa_min PA 
	vector<vector<double>> s{ {}, {} }; // le score du stuff 
	for (int i = pa_min; i <= pa_max; i++)
	{
		r[0].push_back({});
		r[1].push_back({});
		s[0].push_back(0);
		s[1].push_back(0);
	}
	clock_t tps;
	tps = clock();

	for (int i = 0; i < T(items_type[COIFFE]); i++)
	{
		std::cout << i + 1 << "e/" << T(items_type[COIFFE]) << " coiffes : en cours\n";
		for (int j = 0; j < T(items_type[CAPE]); j++)
		{
			std::cout << " " << j + 1 << "e/" << T(items_type[CAPE]) << " capes pour cette coiffe : en cours : " << HMS_EtReset(tps) << " ecoules depuis la precedente \n";
			for (int k = 0; k < T(items_type[ARME]); k++)
			{
				for (int l = 0; l < T(items_type[BOUCLIER]); l++)
				{
					for (int m = 0; m < T(items_type[AMULETTE]); m++)
					{
						for (int n = 0; n < T(items_type[ANNEAU]) - 1; n++)
						{
							for (int o = n + 1; o < T(items_type[ANNEAU]); o++)
							{
								for (int p = 0; p < T(items_type[CEINTURE]); p++)
								{
									for (int q = 0; q < T(items_type[BOTTE]); q++)
									{
										const Cstuff nouv_stuff({
											items_type[COIFFE][i],
											items_type[CAPE][j],
											items_type[ARME][k],
											items_type[BOUCLIER][l],
											items_type[AMULETTE][m],
											items_type[ANNEAU][n],
											items_type[ANNEAU][o],
											items_type[CEINTURE][p],
											items_type[BOTTE][q] });
										//std::cout << "	" << q << " stuff ok\n";
										const Cbonus bonus(nouv_stuff.Gbonus(panos));
										const int rang{ std::min(pa_max, bonus.Gpa_tot()) - pa_min };
										//std::cout << "	" << bonus.Gpa() << " " << rang << "\n";
										if (rang < 0)
										{
											continue;
										}
										const double score{ bonus.Gscore() };
										int pano{ 1 };
										if (nouv_stuff.Gnb_bonus_pano() < 3)
										{
											pano = 0;
										}
										if (score > s[pano][rang])
										{
											s[pano][rang] = score;
											r[pano][rang] = nouv_stuff;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	const str FICHIER_STUFF{ "stuff.txt" };
	std::ofstream st{ FICHIER_STUFF };
	for (int i = 0; i < T(r); i++)
	{
		for (int j = 0; j < T(r[i]); j++)
		{
			if (s[i][j] != 0)
			{
				st << r[i][j].Gprecis(panos) << "\n" << std::endl; // << "		" + std::to_string((int)s[i][j]) : calculé direct dans excel au final 

			}
		}
	}
	std::cout << "detail du stuff copie dans '" << FICHIER_STUFF << "'\n";

	return r;
}


