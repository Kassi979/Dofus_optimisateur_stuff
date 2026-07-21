#pragma once

#include "fonctions.h" 

//const double DIFF_SCORE_TROPHEE_NO_PANO{ 0 };

//extern double MALUS_SCORE_PM;
//extern int NB_PA_VISE;
//const double SCORE_PA{ 220 };

constexpr int NB_MOTS{ 58 }; // le nombre minimum -1 de mots pour former un item ou une pano 

extern vector<double> SCORES;
extern vector<str> NOMS_CARAC;
extern vector<bool> CARAC_SPE;

extern int PA_BASE;
constexpr int PM_BASE{ 3 };
constexpr int PA_MAX{ 12 }, PM_MAX{ 6 }, PO_MAX{ 6 }, P_RES_MAX{ 50 }, P_CRIT_MAX{ 95 };

// les differents mots qui peuvent annoncer une caracteristique 
const vector<str> MOTS_CARAC{ "Renvoi", "PA", "PM" };

// la position de cette carac dans la liste de caracs 
enum POS
{
	VIE = 0, PA, PM, PO, INVOC, 
	PUI, FORCE, INTEL, AGI, CHANCE, DO, DO_N, DO_T, DO_F, DO_A, DO_E, P_CRIT, DO_CRIT, DO_POU, DO_PIEGE, PUI_PIEGE, P_SORT, P_ARME, P_DIST, P_MELEE, 
	RES_N, RES_T, RES_F, RES_A, RES_E, P_RES_N, P_RES_T, P_RES_F, P_RES_A, P_RES_E, RES_CRIT, RES_POU, P_RES_DIST, P_RES_MELEE, 
	RENVOI, SAGE, RET_PA, ESQ_PA, RET_PM, ESQ_PM, SOIN, TACLE, FUITE, INI, PROSP, PODS, 
	AUTRE, 
	NB_CARAC = 52, 

	MAX_PA = 53, MAX_PM, 

	DECALE = 4, // le decalage entre la position dans la liste entre un blonus et dans un item 

};

//extern double SCORE_VIE, SCORE_PUI, SCORE_FORCE, SCORE_INTEL, SCORE_AGI, SCORE_CHANCE, SCORE_DO, SCORE_DO_NEUTRE, SCORE_DO_T, SCORE_DO_F, SCORE_DO_A, SCORE_DO_E, SCORE_PA, SCORE_PM, SCORE_PO, SCORE_INVO;

enum TYPE
{
	COIFFE = 0, CAPE, ARME, BOUCLIER, AMULETTE, ANNEAU, CEINTURE, BOTTE, 
	FIN_TYPE 
};

int StrToType(str const& type)
{
	if (type == "coiffe" || type == "casque" || type == "chapeau")
	{
		return COIFFE;
	}
	if (type == "cape" || type == "sac")
	{
		return CAPE;
	}
	if (type == "arme")
	{
		return ARME;
	}
	if (type == "boucl" || type == "bouclier")
	{
		return BOUCLIER;
	}
	if (type == "amu" || type == "amulette")
	{
		return AMULETTE;
	}
	if (type == "anneau")
	{
		return ANNEAU;
	}
	if (type == "ceint" || type == "ceinture")
	{
		return CEINTURE;
	}
	if (type == "botte" || type == "bottes" || type == "chaussure" || type == "chaussures")
	{
		return BOTTE;
	}
	std::cout << "erreur : type inconnu : '" << type << "'\n";
	return COIFFE;
}

str TypeToStr(int type)
{
	if (type < FIN_TYPE)
	{
		return (vector<str> {"coiffe", "cape", "arme", "bouclier", "amulette", "anneau", "ceinture", "botte"})[type];
	}
	return "autre";
}

// un groupe de quatre elements 
// inutile mtn 
class Celts
{
public:
	Celts(int terre = 0, int feu = 0, int air = 0, int eau = 0)
	{
		mt = terre;
		mf = feu;
		ma = air;
		me = eau;
	}

	Celts& operator+=(Celts const& b)
	{
		mt += b.mt;
		mf += b.mf;
		ma += b.ma;
		me += b.me;

		return *this;
	}

	Celts operator+(Celts const& b) const
	{
		Celts r(*this);
		r += b;
		return r;
	}

	Celts& operator-=(Celts const& b)
	{
		mt -= b.mt;
		mf -= b.mf;
		ma -= b.ma;
		me -= b.me;

		return *this;
	}

	Celts operator-(Celts const& b) const
	{
		Celts r(*this);
		r -= b;
		return r;
	}

	bool operator<=(Celts const& b) const
	{
		return mt <= b.mt && mf <= b.mf && ma <= b.ma && me <= b.me;
	}


	str Gtexte_valeurs() const
	{
		return std::to_string(mt) + "	" + std::to_string(mf) + "	" + std::to_string(ma) + "	" + std::to_string(me);
	}

	int mt, mf, ma, me;
};

class Cbonus
{
public:
	Cbonus() : m_bonus(NB_CARAC, 0)
	{
		m_limite_ou = false;
		m_max_pa = PA_MAX;
		m_max_pm = PM_MAX;
	}

	explicit Cbonus(double score) : Cbonus()
	{
		m_bonus[AUTRE] = (int)score;
	}

	// construction avec une ligne correcte, chaque mot separe correctement 
	explicit Cbonus(vector<str> const& ligne) : m_bonus(NB_CARAC, 0)
	{
		for (int i = 0; i < NB_CARAC; i++)
		{
			m_bonus[i] = StrToInt(ligne[i + DECALE]);
		}
		int pa{ StrToInt(ligne[MAX_PA + DECALE]) }, pm{ StrToInt(ligne[MAX_PM + DECALE]) };
		if (pa > 100)
		{
			pa -= 100;
			pm -= 100;
			m_limite_ou = true;
		}
		else
		{
			m_limite_ou = false;
		}
		m_max_pa = pa;
		m_max_pm = pm;
	}


	double Gscore() const
	{
		vector<int> b{ m_bonus };
		b[PA] += PA_BASE;
		b[PM] += PM_BASE;
		b[PO] = std::min(PO_MAX, b[PO]);
		b[P_CRIT] = std::min(P_CRIT_MAX, b[P_CRIT]);
		b[P_RES_N] = std::min(P_RES_MAX, b[P_RES_N]);
		b[P_RES_T] = std::min(P_RES_MAX, b[P_RES_T]);
		b[P_RES_F] = std::min(P_RES_MAX, b[P_RES_F]);
		b[P_RES_A] = std::min(P_RES_MAX, b[P_RES_A]);
		b[P_RES_E] = std::min(P_RES_MAX, b[P_RES_E]);
		if (m_limite_ou && b[PA] > m_max_pa && b[PM] > m_max_pm)
		{
			b[PA] = std::min(PA_MAX, b[PA]);
			b[PM] = std::min(PM_MAX, b[PM]);
			vector<int> c{ b };
			b[PA] = std::min(m_max_pa, b[PA]);
			c[PM] = std::min(m_max_pm, c[PM]);
			return std::max(ProdScal(b, SCORES), ProdScal(c, SCORES));
		}
		else
		{
			b[PA] = std::min(m_max_pa, b[PA]);
			b[PM] = std::min(m_max_pm, b[PM]);
			return ProdScal(b, SCORES);
		}
	}


	str Gtexte() const
	{
		str r{ "" };
		for (int i = 0; i < T(m_bonus); i++)
		{
			if ((SCORES[i] > 0 || CARAC_SPE[i]) && m_bonus[i] > 0)
			{
				r += std::to_string(m_bonus[i]) + " " + NOMS_CARAC[i] + ", ";
			}
		}
		r += " -> " + std::to_string(Gscore());
		return r;
		//return "vie " + std::to_string(m_vie) + "	stats " + std::to_string(m_stat) + "	do " + std::to_string(m_do) + "	PA " + std::to_string(m_pa) + "	PM " + std::to_string(m_pm) + "	 + " + std::to_string(m_bonus) + "	-> " + std::to_string((int)m_score);
	}

	void Aff() const
	{
		std::cout << this->Gtexte() << "\n";
	}

	// les nb bien positionnes : permet d'exporter dans excel 
	str Gtexte_valeurs() const
	{
		str r{ "" };
		for (int i = 0; i < NB_CARAC; i++)
		{
			r += std::to_string(m_bonus[i]) + "	";
		}
		r += "	";
		if (m_limite_ou)
		{
			r += std::to_string(m_max_pa + 100) + "	" + std::to_string(m_max_pm + 100);
		}
		else
		{
			r += std::to_string(m_max_pa) + "	" + std::to_string(m_max_pm);
		}
		return r;
	}

	int G(int bonus) const
	{
		return m_bonus[bonus];
	}

	// renvoi si l'item n'impose pas PA < PA_min_voulu ET PM < PM_min_voulu
	bool PA_PM_OK(int PA_min_voulu, int PM_min_voulu) const
	{
		if (m_limite_ou)
		{
			return (m_max_pa >= PA_min_voulu) || (m_max_pm >= PM_min_voulu);
		}
		return m_max_pa >= PA_min_voulu && m_max_pm >= PM_min_voulu;
	}

	int Gpa() const
	{
		return m_bonus[PA];
	}

	// prend en compte les PA de base du perso 
	int Gpa_tot() const
	{
		const int pa{ m_bonus[PA] + PA_BASE };
		if (m_limite_ou)
		{
			if (m_bonus[PM] + PM_BASE <= m_max_pm)
			{
				return std::min(pa, PA_MAX);
			}
		}
		return std::min(pa, m_max_pa);
	}



	// renvoi si cet item est mieux que b, en regardant uniquement les stats speciales et le score 
	// ne verifie pas les limitations de PA et PM car normalement on a retire tous ceux qui ne respectent pas !
	bool MieuxSpe(Cbonus const& b) const
	{
		for (int i = 0; i < NB_CARAC; i++)
		{
			if (CARAC_SPE[i] && m_bonus[i] < b.m_bonus[i])
			{
				return false;
			}
		}
		return Gscore() >= b.Gscore();
	}

	// renvoi si b est mieux que cet item, en regardant uniquement les stats speciales et le score 
	bool InfSpe(Cbonus const& b) const
	{
		return !(*this).MieuxSpe(b);
	}



	Cbonus& operator+=(Cbonus const& b)
	{
		AjVect(m_bonus, b.m_bonus);
		
		if (m_limite_ou && !b.m_limite_ou)
		{
			if (m_max_pa >= b.m_max_pa || m_max_pm >= b.m_max_pm)
			{
				// l'autre a une limite et, et il est plus restrictif 
				// bon la on est un peu dur, je pense pas que ca change grand chose en vrai ...
				m_limite_ou = false;
			}
		}
		else if (b.m_limite_ou && !m_limite_ou)
		{
			if (m_max_pa > b.m_max_pa && m_max_pm > b.m_max_pm)
			{
				m_limite_ou = true; // ici pareil c'est pas completement vrai mais bon on va dire que ca va ... 
			}
		}
		m_max_pa = std::min(m_max_pa, b.m_max_pa);
		m_max_pm = std::min(m_max_pm, b.m_max_pm);

		return *this;
	}

	Cbonus operator+(Cbonus const& b) const 
	{
		Cbonus a(*this);
		a += b;
		return a;
	}

	Cbonus& operator-=(Cbonus const& b)
	{
		RetVect(m_bonus, b.m_bonus);

		// pas besoin de modifier les limites pour la soustraction 

		return *this;
	}

	Cbonus operator-(Cbonus const& b) const
	{
		Cbonus a(*this);
		a -= b;
		return a;
	}


	// mots est la liste de mots du bonus de stats, non vide 
	void MajBonus(vector<str> const& mots)
	{
		if (mots[0] == "Renvoi" && T(mots) > 2)
		{
			//std::cout << Phrase(mots) << "\n";
			int nb{ StrToInt(mots[1]) };
			if (T(mots) > 4 && mots[2] == "a")
			{
				nb = StrToInt(mots[3]);
			}
			m_bonus[RENVOI] = nb;
			return;
		}
		else if (mots[0] == "PA" && T(mots) > 2 && mots[1] == "<")
		{
			m_max_pa = StrToInt(mots[2]) - 1;
			if (T(mots) > 6 && mots[3] == "ou" && mots[4] == "PM" && mots[5] == "<")
			{
				m_max_pm = StrToInt(mots[6]) - 1;
				m_limite_ou = true;
			}
			return;
		}
		else if (mots[0] == "PM" && T(mots) > 2 && mots[1] == "<")
		{
			m_max_pm = StrToInt(mots[2]) - 1;
			return;
		}

		int nb{ StrToInt(mots[0]) };

		int p = 1; // l'endroit ou se trouve le nom de la stat qui a ce jet 
		if (mots[1] == "a" && T(mots) > 3)
		{
			nb = StrToInt(mots[2]);
			p = 3;
		}

		//std::cout << T(mots[1]) << " " << mots[1][0] << " " << "à"[0] << " " << "à" << " " << (mots[1] == "à") << " " << true;

		//std::cout << "place de la stat : " << p << " ; jet : " << nb << "\n";
		if (mots[p] == "Vitalite")
		{
			m_bonus[VIE] = nb;
			//std::cout << "Vita\n";
		}
		else if (mots[p] == "PA")
		{
			m_bonus[PA] = nb;
		}
		else if (mots[p] == "PM")
		{
			if (T(mots) == p + 1)
			{
				m_bonus[PM] = nb;
			}
			else if (mots[p + 1] == "Max")
			{
				m_max_pm = nb;
			}
		}
		else if (mots[p] == "PO")
		{
			m_bonus[PO] = nb;
		}
		else if (mots[p] == "Invocation")
		{
			m_bonus[INVOC] = nb;
		}
		else if (mots[p] == "Puissance")
		{
			if (T(mots) > p + 1 && mots[p + 1] == "Piege")
			{
				m_bonus[PUI_PIEGE] = nb;
			}
			else
			{
				m_bonus[PUI] = nb;
			}
		}
		else if (mots[p] == "Force")
		{
			m_bonus[FORCE] = nb;
		}
		else if (mots[p] == "Intelligence")
		{
			m_bonus[INTEL] = nb;
		}
		else if (mots[p] == "Agilite")
		{
			m_bonus[AGI] = nb;
		}
		else if (mots[p] == "Chance")
		{
			m_bonus[CHANCE] = nb;
		}
		else if (mots[p] == "Dommages")
		{
			if (T(mots) == p + 1)
			{
				m_bonus[DO] = nb;
			}
			else if (mots[p + 1] == "Piege")
			{
				m_bonus[DO_PIEGE] = nb;
			}
		}
		else if (mots[p] == "Dmg" && T(mots) > p + 1)
		{
			if (mots[p + 1] == "Neutre")
			{
				m_bonus[DO_N] = nb;
			}
			else if (mots[p + 1] == "Terre")
			{
				m_bonus[DO_T] = nb;
			}
			else if (mots[p + 1] == "Feu")
			{
				m_bonus[DO_F] = nb;
			}
			else if (mots[p + 1] == "Air")
			{
				m_bonus[DO_A] = nb;
			}
			else if (mots[p + 1] == "Eau")
			{
				m_bonus[DO_E] = nb;
			}
			else if (mots[p + 1] == "Critique")
			{
				m_bonus[DO_CRIT] = nb;
			}
			else if (mots[p + 1] == "Poussee")
			{
				m_bonus[DO_POU] = nb;
			}
		}
		else if (mots[p] == "%" && T(mots) > p + 1)
		{
			if (mots[p + 1] == "Critique")
			{
				m_bonus[P_CRIT] = nb;
			}
			else if (mots[p + 1] == "Dmg" && T(mots) > p + 2)
			{
				if (mots[p + 2] == "d'Armes")
				{
					m_bonus[P_ARME] = nb;
				}
				else if (mots[p + 2] == "Distance")
				{
					m_bonus[P_DIST] = nb;
				}
				else if (mots[p + 2] == "Melee")
				{
					m_bonus[P_MELEE] = nb;
				}
				else if (mots[p + 2] == "aux") // "Sorts" 
				{
					m_bonus[P_SORT] = nb;
				}
			}
			else if (mots[p + 1] == "Res." && T(mots) > p + 2)
			{
				if (mots[p + 2] == "Neutre")
				{
					m_bonus[P_RES_N] = nb;
				}
				else if (mots[p + 2] == "Terre")
				{
					m_bonus[P_RES_T] = nb;
				}
				else if (mots[p + 2] == "Feu")
				{
					m_bonus[P_RES_F] = nb;
				}
				else if (mots[p + 2] == "Air")
				{
					m_bonus[P_RES_A] = nb;
				}
				else if (mots[p + 2] == "Eau")
				{
					m_bonus[P_RES_E] = nb;
				}
				else if (mots[p + 2] == "Distance")
				{
					m_bonus[P_RES_DIST] = nb;
				}
				else if (mots[p + 2] == "Melee")
				{
					m_bonus[P_RES_MELEE] = nb;
				}
			}
		}
		else if (mots[p] == "Res." && T(mots) > p + 1)
		{
			if (mots[p+1] == "Neutre")
			{
				m_bonus[RES_N] = nb;
			}
			else if (mots[p + 1] == "Terre")
			{
				m_bonus[RES_T] = nb;
			}
			else if (mots[p + 1] == "Feu")
			{
				m_bonus[RES_F] = nb;
			}
			else if (mots[p + 1] == "Air")
			{
				m_bonus[RES_A] = nb;
			}
			else if (mots[p + 1] == "Eau")
			{
				m_bonus[RES_E] = nb;
			}
			else if (mots[p + 1] == "Critique")
			{
				m_bonus[RES_CRIT] = nb;
			}
			else if (mots[p + 1] == "Poussee")
			{
				m_bonus[RES_POU] = nb;
			}
		}
		else if (mots[p] == "Sagesse")
		{
			m_bonus[SAGE] = nb;
		}
		else if (mots[p] == "Retrait" && T(mots) > p + 1)
		{
			if (mots[p + 1] == "PA")
			{
				m_bonus[RET_PA] = nb;
			}
			else if (mots[p + 1] == "PM")
			{
				m_bonus[RET_PM] = nb;
			}
		}
		else if (mots[p] == "Esquive" && T(mots) > p + 1)
		{
			if (mots[p + 1] == "PA")
			{
				m_bonus[ESQ_PA] = nb;
			}
			else if (mots[p + 1] == "PM")
			{
				m_bonus[ESQ_PM] = nb ;
			}
		}
		else if (mots[p] == "Soin")
		{
			m_bonus[SOIN] = nb;
		}
		else if (mots[p] == "Tacle")
		{
			m_bonus[TACLE] = nb;
		}
		else if (mots[p] == "Fuite")
		{
			m_bonus[FUITE] = nb;
		}
		else if (mots[p] == "Initiative")
		{
			m_bonus[INI] = nb;
		}
		else if (mots[p] == "Prospection")
		{
			m_bonus[PROSP] = nb;
		}
		else if (mots[p] == "Pods")
		{
			m_bonus[PODS] = nb;
		}
	}

	friend void CreeItems(str const& fichier);

protected:
	vector<int> m_bonus;
	int m_max_pa, m_max_pm;
	bool m_limite_ou; // si la limitation est l'un OU l'autre 
};

class Citem : public Cbonus 
{
public:
	Citem() : Cbonus() 
	{
		m_nom = "inconnu";
		m_pano = "";
		m_type = COIFFE;
		m_niveau = -1;
	}

	Citem(str const& nom, int type, Cbonus const& bonus, int niv = -1, str const& pano = "") : Cbonus(bonus)
	{
		m_nom = nom;
		m_pano = pano;
		m_type = type;
		m_niveau = niv;
	}


	str G() const
	{
		return m_nom + " (" + std::to_string((int)Gscore()) + ")";
	}

	str Gtexte() const
	{
		return m_nom + " : " + this->Cbonus::Gtexte();
	}

	// permet d'exporter en excel 
	str Gprecis() const
	{
		return m_pano + "	" + m_nom + "	" + TypeToStr(m_type) + "	" + std::to_string(m_niveau) + "	" + this->Gtexte_valeurs();
	}

	str Gnom() const
	{
		return m_nom;
	}

	str Gpano() const
	{
		return m_pano;
	}

	bool EstDePano(str const& pano) const 
	{
		return m_pano == pano;
	}

	int Gtype() const
	{
		return m_type;
	}

	int Gniv() const
	{
		return m_niveau;
	}


	Citem operator+(Cbonus const& b) const
	{
		Citem a(*this);
		a.Cbonus::operator+=(b);
		return a;
	}

	// compare avec les stats speciales 
	// inutile vu que ca existe deja dans Cbonus 
	bool InfSpe(Citem const& b) const
	{
		return b.MieuxSpe(*this);
	}


	

	friend void CreeItems(str const& fichier);

private:
	str m_nom;
	str m_pano;
	int m_type;
	int m_niveau;
};


void AffItems(vector<vector<Citem>> const& items_type, str const& message_debut = "items : \n", str const& fin_affiche = "\n")
{
	std::cout << message_debut;
	int tot{ 0 };
	for (int i = 0; i < T(items_type); i++)
	{
		tot += T(items_type[i]);
		std::cout << T(items_type[i]) << " " << TypeToStr(i) << "\n";
	}
	std::cout << "total : " << tot << "\n";
	std::cout << fin_affiche;
}

bool DebugChercheItem(str const& nom, vector<vector<Citem>> const& items_type)
{
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (items_type[i][j].Gnom() == nom)
			{
				std::cout << nom << " trouve !\n";
				return true;
			}
		}
	}
	std::cout << nom << " pas trouve ...\n";
	return false;
}

bool DebugPosItem(str const& nom, vector<vector<Citem>> const& items_type, int& type, int& pos)
{
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (items_type[i][j].Gnom() == nom)
			{
				std::cout << nom << " trouve !\n";
				type = i;
				pos = j;
				return true;
			}
		}
	}
	std::cout << nom << " pas trouve ...\n";
	return false;
}


class Cpano
{
public:
	Cpano() : m_bonus({}) 
	{
		m_nom = "pano inconnue";
	}

	Cpano(str const& nom) : m_bonus({})
	{
		m_nom = nom;
	}

	Cpano& Aj(Cbonus const& bonus, int pour_x_items)
	{
		pour_x_items--; // on veut le mettre en position -1 
		if (std::empty(m_bonus))
		{
			m_bonus.push_back(Cbonus());
		}
		while (T(m_bonus) <= pour_x_items)
		{
			m_bonus.push_back(m_bonus[T(m_bonus) - 1]);
		}
		m_bonus[pour_x_items] = bonus;

		return *this;
	}


	str Gnom() const
	{
		return m_nom;
	}

	str Gligne(int nb_items) const
	{
		if (nb_items > T(m_bonus) || nb_items < 1)
		{
			std::cout << "erreur : cherche une pano inexistante : " << m_nom << " pour " << nb_items << " items\n";
		}
		return m_nom + "	" + std::to_string(nb_items) + "			" + m_bonus[nb_items - 1].Gtexte_valeurs() + "\n";

	}

	// export excel 
	str Gprecis() const
	{
		str r{ "" };
		for (int i = 2; i <= T(m_bonus); i++)
		{
			r += Gligne(i);
		}
		return r;
	}


	Cbonus Gbonus(int pour_x_items) const
	{
		if (std::empty(m_bonus) || pour_x_items <= 0)
		{
			return Cbonus();
		}
		pour_x_items--;
		if (pour_x_items < T(m_bonus))
		{
			return m_bonus[pour_x_items];
		}
		return m_bonus[T(m_bonus) - 1];
	}

	double Gscore(int pour_x_items) const
	{
		return this->Gbonus(pour_x_items).Gscore();
	}


	Cbonus Gbonus_max() const
	{
		return this->Gbonus(T(m_bonus));
	}

	// donne le bonus maximum grace a un item en plus sur la pano 
	// item_max à -1 pour donner sur tous les nombres d'items possibles 
	double Gscore_max_un_item(int item_max = -1) const
	{
		if (item_max == 0)
		{
			return 0 ;
		}
		double r{ 0 }, prec{ 0 };
		int t{ T(m_bonus) };
		if (item_max != -1)
		{
			t = std::min(item_max, t);
		}
		for (int i = 0; i < t; i++)
		{
			const double b{ m_bonus[i].Gscore() };
			r = std::max(r, b - prec);
			prec = b;
		}
		return r;
	}


	// donne le bonus lorsqu'on ajoute ce numero item de la pano 
	Cbonus Gbonus_ajoute_item(int ajoute_item) const
	{
		return this->Gbonus(ajoute_item) - this->Gbonus(ajoute_item - 1);
	}

	// le bonus que peut donner un unique item de cette pano, sachant qu'on ne peut pas depasser max_item 
	vector<Cbonus> Gtous_bonus_jusqua(int max_item, int item_min = 1) const
	{
		vector<Cbonus> r{};
		max_item = std::min(max_item, T(m_bonus) - 1);
		for (int i = item_min; i <= max_item; i++)
		{
			r.push_back(Gbonus_ajoute_item(i));
		}
		return r;
	}

	// donne si cette pano est forcement moins bien que b (au niveau des bonus uniquement) 
	bool InfSpe(Cpano const& b, int items_max = -1) const 
	{
		int t1{ T(m_bonus) }, t2{ T(b.m_bonus) };
		if (items_max != -1)
		{
			t1 = std::min(t1, items_max);
			t2 = std::min(t2, items_max);
		}
		if (t1 > t2)
		{
			return false;
		}
		for (int i = 0; i < t1; i++)
		{
			if (!b.m_bonus[i].MieuxSpe(m_bonus[i]))
			{
				return false;
			}
		}
		return true;
	}




	friend void CreePanos(str const& fichier);
	

private:
	str m_nom;
	vector<Cbonus> m_bonus; // le bonus pour tel nombre d'items : commence à 1 items 
};

Cpano Pano(Citem const& item, vector<Cpano> const& panos)
{
	for (int i = 0; i < T(panos); i++)
	{
		if (item.EstDePano(panos[i].Gnom()))
		{
			return panos[i];
		}
	}
	return Cpano("aucune");
}

bool APano(Citem const& item, vector<Cpano> const& panos, int& pos)
{
	for (int i = 0; i < T(panos); i++)
	{
		if (item.EstDePano(panos[i].Gnom()))
		{
			pos = i;
			return true;
		}
	}
	return false;
}


// permet d'exporter une liste d'items copiés de dofus book dans une tableau excel en separant chaque stat 
// il faut remplacer tous les "à" par "a" et tous les "é" par "e", et les "â" par "a" 
void CreeItems(str const& fichier)
{
	const str nouv_fichier{ "formate_nouveaux_" + fichier };
	std::ifstream s{ fichier };
	std::ofstream e{ nouv_fichier };
	str l{ "" };
	int item_etape{ 0 }; // 0 : pas de nom, 1 : a un nom, 2 : a un type, 3 : a une pano, 4 a des stats 
	Citem item;
	int nb{ 0 };
	while (std::getline(s, l))
	{
		if (l == "")
		{
			continue;
		}

		RemplaceAccents(l);

		vector<str> mots = Mots(l, ' ');
		if (StrToInt(mots[0], nb) || Parmi(mots[0], MOTS_CARAC))
		{
			if (item_etape >= 2 && T(mots) > 1)
			{
				item_etape = 4;

				//Pause(Phrase(mots) + " : ajout de stats\n");
				
				item.MajBonus(mots);
				
			}
		}
		else
		{
			if (T(mots) > 3 && mots[2] == "Niveau")
			{
				//Pause(Phrase(mots) + " : ajout de type et niveau\n");

				// (vector<str> {"coiffe", "cape", "arme", "bouclier", "amulette", "anneau", "ceinture", "botte"})[type];
				int pos{ 0 };
				if (ParmiPos(mots[0], { "Chapeau", "Cape", "Arme = spe", "Bouclier", "Amulette", "Anneau", "Ceinture", "Bottes" }, pos))
				{
					item.m_type = pos;
				}
				else if (Parmi(mots[0], { "Hache", "Faux", "Pioche", "Marteau", "Pelle", "Dagues", "Arc", "Epee", "Baton", "Baguette", "Lance" }))
				{
					item.m_type = ARME;
				}
				else
				{
					std::cout << "erreur : creation d'item sans type !\n";
				}
				item.m_niveau = StrToInt(mots[3]);

				item_etape = 2;
			}
			else if (item_etape == 2)
			{
				//Pause(Phrase(mots) + " : ajout de pano\n");
				item.m_pano = Phrase(mots);
				item_etape = 3;
			}
			else 
			{
				if (item_etape > 2)
				{
					e << item.Gprecis() << "\n";
					//std::cout << item.Gprecis() << "\n";
				}
				//Pause(Phrase(mots) + " : nouvel item\n");
				item = Citem();
				item.m_nom = Phrase(mots);
				item_etape = 1;
			}
		}
	}
	if (item_etape > 2)
	{
		e << item.Gprecis() << "\n";
		//std::cout << item.Gprecis() << "\n";
	}
	std::cout << "items exportes dans \"" << nouv_fichier << "\"\n";
}

// permet d'exporter une liste de panos copiés de dofus book dans une tableau excel en separant chaque stat 
// il faut remplacer tous les "à" par "a", tous les "é" par "e", et les "â" par "a" 
void CreePanos(str const& fichier)
{
	const str nouv_fichier{ "formate_nouvelles_" + fichier };
	std::ifstream s{ fichier };
	std::ofstream e{ nouv_fichier };
	str l{ "" };
	int pano_etape{ 0 }; // 0 : pas de nom, 1 : a un nom, 2 : a des stats 
	int nb_items{ 0 };
	str prec{ "" };
	Cpano pano;
	Cbonus bonus;
	int nb{ 0 };
	while (std::getline(s, l))
	{
		if (l == "")
		{
			continue;
		}
		RemplaceAccents(l);
		//std::cout << l << "\n";

		vector<str> mots = Mots(l, ' ');
		if (StrToInt(mots[0], nb) || Parmi(mots[0], MOTS_CARAC))
		{
			if (pano_etape >= 2 && T(mots) > 1)
			{
				pano_etape = 2;

				//Pause(Phrase(mots) + " : ajout de stats\n");

				bonus.MajBonus(mots);

			}
		}
		else if (mots[0] == "Bonus" && T(mots) > 2 && mots[1] != "panoplie")
		{
			if (pano_etape > 1)
			{
				pano.Aj(bonus, nb_items);
			}
			bonus = Cbonus();
			nb_items = StrToInt(mots[1]);
			pano_etape = 2;
		}
		else
		{
			if (T(mots) > 2 && mots[1] == "Niveau")
			{
				if (pano_etape > 1)
				{
					pano.Aj(bonus, nb_items);
					e << pano.Gprecis();
					//std::cout << pano.Gprecis();
					//Pause();
				}
				pano = Cpano(prec);
				bonus = Cbonus();
				pano_etape = 1;
			}
			else
			{
				prec = Phrase(mots);
			}
		}
	}
	if (pano_etape > 1)
	{
		pano.Aj(bonus, nb_items);
		e << pano.Gprecis();
	}
	std::cout << "panos exportees dans \"" << nouv_fichier << "\"\n";
}



void ExporteItems(str const& fichier, vector<vector<Citem>>& items_type)
{
	std::ofstream s{ fichier };
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			s << items_type[i][j].Gprecis() << "\n";
		}
	}
	std::cout << "items exportes dans \"" + fichier + "\"\n";
}

// fonctions pour supprimer les objets inutiles 
// 
// - juste avec le score marche trop bien : supprime des objets qui pourraient etre utiles avec le calcul du score total du stuff en prenant en compte le cout 
// - avec toutes les stats marche pas assez : supprime trop peu d'objets 
// - special : avec une fonction de comparaison specialement concue pour ne pas minimiser le calcul du score du stuff : ca semble le mieux, mais ca doit etre ameliorable 


// retire tous les objets qui depassent ce niveau 
void ClearItemsNiv(vector<vector<Citem>>& items_type, int niv_max)
{
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (items_type[i][j].Gniv() > niv_max)
			{
				Supp_(items_type[i], j);
				j--;
			}
		}
	}
}

void ClearItemsPA_PM(vector<vector<Citem>>& items_type, int PA_min_voulu, int PM_min_voulu)
{
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (!items_type[i][j].PA_PM_OK(PA_min_voulu, PM_min_voulu))
			{
				Supp_(items_type[i], j);
				j--;
			}
		}
	}
}

// pour cette liste d'items, donne le maximum d'objets dans chaque pano 
vector<int> CompteItems(vector<Cpano> const& panos, vector<vector<Citem>> const& items_type)
{
	vector<int> nb_items{ vector<int>(T(panos), 0) }; // on va compter le nombre max d'item pour chaque pano 
	int pos{ 0 };
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (APano(items_type[i][j], panos, pos))
			{
				//std::cout << T(nb_items) << " " << pos << "\n";
				nb_items[pos]++;
			}
		}
	}
	return nb_items;
}

// donne pour chaque pano : 
// nb_items[pano] le nb d'items 
// pos_items[pano] est un vector { type, pos } des items de cette pano 
void ChercheItems(vector<Cpano> const& panos, vector<vector<Citem>> const& items_type, vector<int>& nb_items, vector<vector<vector<int>>>& pos_items)
{
	nb_items = vector<int>(T(panos), 0); // on va compter le nombre max d'item pour chaque pano 
	pos_items = vector<vector<vector<int>>>(T(panos), vector<vector<int>> {}); 
	int pos{ 0 };
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (APano(items_type[i][j], panos, pos))
			{
				//std::cout << T(nb_items) << " " << pos << "\n";
				nb_items[pos]++;
				pos_items[pos].push_back({ i, j });
			}
		}
	}
}

// donne un vecteur avec en [x] le meilleur item respectant x 
// x vaut : 0 sans pano ; 1 sans pano avec PA ; 2 avec pano ; 3 avec pano avec PA 
// et n'ayant pas de restriction de PA/PM 
// on remarque qu'un item avec pano est forcement au moins mieux que sans (c'est un inconvenient d'avoir une pano, et donc un item sans pano peut prendre sa place) 
vector<int> MeilleurItem(vector<Citem> const& items)
{
	vector<int> meilleur(4, 0);
	vector<double> meilleur_score(4, 0);

	for (int j = 0; j < T(items); j++)
	{
		const double s{ items[j].Gscore() };
		// avec pano : peut etre pris par un item sans pano 
		if (s > meilleur_score[2] && items[j].Gpa() > -1)
		{
			meilleur_score[2] = s;
			meilleur[2] = j;
		}
		if (s > meilleur_score[3] && items[j].Gpa() > 0)
		{
			meilleur_score[3] = s;
			meilleur[3] = j;
		}
		if (items[j].Gpano() == "")
		{
			// sans pano 
			// ici on pourrait optimiser pour les non-anneaux : un items qui est le seul de sa pano compte comme n'ayant pas de pano 
			if (s > meilleur_score[0] && items[j].Gpa() > -1)
			{
				meilleur_score[0] = s;
				meilleur[0] = j;
			}
			if (s > meilleur_score[1] && items[j].Gpa() > 0)
			{
				meilleur_score[1] = s;
				meilleur[1] = j;
			}
		}
		
	}
	return meilleur;
}

// donne un vecteur avec en [x] le meilleur item respectant x 
// x vaut : 0 sans pano ; 1 sans pano avec PA ; 2 avec pano ; 3 avec pano avec PA 
// on remarque qu'un item avec pano est forcement au moins mieux que sans (c'est un inconvenient d'avoir une pano, et donc un item sans pano peut prendre sa place) 
// optimisé en regardant si un item non anneau est le seul de sa pano : du coup plus lent dans le calcul 
vector<int> MeilleurItemOpti(vector<Citem> const& items, vector<Cpano> const& panos, vector<int> const& nb_items_pano)
{
	vector<int> meilleur(4, 0);
	vector<double> meilleur_score(4, 0);

	for (int j = 0; j < T(items); j++)
	{
		const double s{ items[j].Gscore() };
		// avec pano : peut etre pris par un item sans pano 
		if (s > meilleur_score[2] && items[j].Gpa() > -1)
		{
			meilleur_score[2] = s;
			meilleur[2] = j;
		}
		if (s > meilleur_score[3] && items[j].Gpa() > 0)
		{
			meilleur_score[3] = s;
			meilleur[3] = j;
		}
		int pos_pano{ 0 };
		bool no_pano{ true };
		if (APano(items[j], panos, pos_pano))
		{
			if (nb_items_pano[pos_pano] > 1 || items[j].Gtype() == ANNEAU)
			{
				no_pano = false;
			}
		}
		if (no_pano)
		{
			// sans pano 
			if (s > meilleur_score[0] && items[j].Gpa() > -1)
			{
				meilleur_score[0] = s;
				meilleur[0] = j;
			}
			if (s > meilleur_score[1] && items[j].Gpa() > 0)
			{
				meilleur_score[1] = s;
				meilleur[1] = j;
			}
		}

	}
	return meilleur;
}

// clear les items qui sont moins bien que le meilleur item sans pano, quel que soit leur bonus de pano et en respectant les stats speciales 
// clear aussi les items de pano qui sont moins bien qu'un item de pano 
// bon pour l'instant ya que pa qui est compte comme une stat speciale ...
// ok c'est plutot pas mal ca clear bien 
bool Clear_1(vector<vector<Citem>>& items_type, vector<Cpano> const& panos, bool texte = true)
{
	vector<int> nb_items{ CompteItems(panos, items_type) }; // on va compter le nombre max d'item pour chaque pano 
	//vector<vector<bool>> supp{};
	bool un_supp{ false };
	int nb_supp{ 0 };
	if (texte)
	{
		std::cout << "clear d'item 1 par 1 ...\n";
	}

	for (int i = 0; i < T(items_type); i++)
	{
		vector<bool> supp(T(items_type[i]), false);

		//vector<int> meilleur{ MeilleurItem(items_type[i]) };
		const vector<int> meilleur{ MeilleurItemOpti(items_type[i], panos, nb_items) };
		if (texte)
		{
			std::cout << TypeToStr(i) << " :\n";
			std::cout << "meilleur item : " << items_type[i][meilleur[0]].Gtexte() << "\n";
			std::cout << "meilleur item PA : " << items_type[i][meilleur[1]].Gtexte() << "\n"; // en fait ya pa bcp d'item pa de base donc ca enleve pas grand chose dans les panos 
			std::cout << "meilleur item pano : " << items_type[i][meilleur[2]].Gtexte() << "\n";
			std::cout << "meilleur item pano PA : " << items_type[i][meilleur[3]].Gtexte() << "\n\n";
		}

		// retire tous les items moins bien quel que soit leur bonnus de pano 
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (Parmi(j, meilleur))
			{
				continue;
			}

			//std::cout << items_type[i][j].Gtexte() << "\n";

			int pos_pano{ 0 }, m1{ meilleur[0] }, m2{ meilleur[1] };
			if (items_type[i][j].InfSpe(items_type[i][m1]) || items_type[i][j].InfSpe(items_type[i][m2])) // avant de tester si il est meilleur avec une pano, on doit le comparer uniquement aux objets sans pano 
			{
				//std::cout << j << " : " << items_type[i][j].Gnom() << " moins bien sans pano\n";
				bool suppr{ true };

				const bool a_pano{ APano(items_type[i][j], panos, pos_pano) };
				if (a_pano && i != ANNEAU)
				{
					m1 = meilleur[2]; // un anneau de pano ne peut pas etre supp par un autre anneau de pano : il en faut deux !
					m2 = meilleur[3];
				}
				if (a_pano)
				{
					vector<Cbonus> b{ panos[pos_pano].Gtous_bonus_jusqua(nb_items[pos_pano], 2) };
					for (int k = 0; k < T(b); k++)
					{
						Cbonus avec_pano{ items_type[i][j] + b[k] };
						//std::cout << panos[pos_pano].Gnom() << " " << b[k].Gtexte() << "\n";
						if (!items_type[i][m1].MieuxSpe(avec_pano) && !items_type[i][m2].MieuxSpe(avec_pano)) // ici on est sur qu'il a un bonus de pano : on peut mettre des objets de pano 
						{
							//std::cout << "pas moins bien avec pano\n";
							suppr = false; // il est pas moins bien que les deux avec ce bonus de pano 
							break;
						}
					}
					//Pause();
				}
				supp[j] = suppr;
				//Pause();
			}
		}
		// on retire les items 
		for (int j = 0; j < T(items_type[i]); j++)
		{
			//std::cout << j << " : "  << supp[j] << "\n";
			if (supp[j])
			{
				un_supp = true;
				Supp_(items_type[i], j);
				Supp_(supp, j);
				j--;
				nb_supp++;
				//std::cout << nb_supp << "\n";
			}
		}
		//std::cout << nb_supp << "\n";
		//Pause();
	}
	if (texte && un_supp)
	{
		std::cout << "suppression de " << nb_supp << " items, il reste : \n";
		AffItems(items_type, "");
		Pause();
	}
	else if (texte)
	{
		std::cout << "rien n'a ete suppr ...\n";
	}
	return un_supp;
}

// ajoute les anneaux sans pano qui restent 
void AjAnneaux(vector<vector<Citem>>& items_type)
{
	const int t{ T(items_type[ANNEAU]) };
	for (int i = 0; i < t; i++)
	{
		if (items_type[ANNEAU][i].Gpano() == "")
		{
			items_type[ANNEAU].push_back(items_type[ANNEAU][i]);
		}
	}
}

// si une pano ayant au moins les items d'une autre pano a des bonus superieurs sur chaque item et sur chaque bonus de pano, elle est strictement mieux 
// renvoie vrai si il a suppr au moins un item 
bool ClearPano(vector<vector<Citem>>& items_type, vector<Cpano> const& panos, bool texte = true)
{
	if (texte)
	{
		std::cout << "suppression de panos ...\n";
	}
	vector<int> nb_items{}; // on va compter le nombre max d'item pour chaque pano 
	vector<vector<vector<int>>> pos_items{};
	ChercheItems(panos, items_type, nb_items, pos_items);
	vector<vector<bool>> supp{};
	for (int i = 0; i < T(items_type); i++)
	{
		supp.push_back(vector<bool>(T(items_type[i]), false));
	}
	bool trouve{ false }; // on a trouve une pano a suppr
	for (int i = 0; i < T(panos); i++)
	{
		if (nb_items[i] < 2)
		{
			continue;
		}
		for (int j = 0; j < T(panos); j++)
		{
			if (j == i)
			{
				continue;
			}
			// on va regarder si i se fait battre par j 
			if (nb_items[i] <= nb_items[j] && panos[i].InfSpe(panos[j], nb_items[i]))
			{
				bool pano_supp{ true };
				for (int k = 0; k < T(pos_items[i]); k++)
				{
					bool item_supp{ false };
					for (int l = 0; l < T(pos_items[j]); l++)
					{
						if (pos_items[i][k][0] != pos_items[j][l][0])
						{
							continue;
						}
						// on a deux items du meme type : on les compare 
						if (items_type[pos_items[i][k][0]][pos_items[i][k][1]].InfSpe(items_type[pos_items[j][l][0]][pos_items[j][l][1]]))
						{
							item_supp = true;
							break;
						}
					}
					if (!item_supp)
					{
						pano_supp = false;
						break;
					}
				}
				if (pano_supp)
				{
					if (texte)
					{
						std::cout << panos[i].Gnom() << " (" << nb_items[i] << " items) <= " << panos[j].Gnom() << "\n";
					}
					trouve = true;
					for (int k = 0; k < T(pos_items[i]); k++)
					{
						supp[pos_items[i][k][0]][pos_items[i][k][1]] = true;
					}
					break;
				}
			}
		}
	}
	int nb_supp{ 0 };
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			//std::cout << j << " : "  << supp[j] << "\n";
			if (supp[i][j])
			{
				Supp_(items_type[i], j);
				Supp_(supp[i], j);
				j--;
				nb_supp++;
				//std::cout << nb_supp << "\n";
			}
		}
	}
	if (texte && trouve)
	{
		std::cout << "suppression de " << nb_supp << " items, il reste : \n";
		AffItems(items_type, "");
		Pause();
	}
	else if (texte)
	{
		std::cout << "rien n'a ete suppr\n";
	}
	return trouve;
	if (trouve)
	{
		ClearPano(items_type, panos, texte);
	}
}

bool Clear_2(vector<vector<Citem>>& items_type, vector<Cpano> const& panos, bool texte = true)
{
	if (texte)
	{
		std::cout << "clear d'item en faisant des paires d'items de meme pano ...\n";
	}
	vector<int> nb_items{ CompteItems(panos, items_type) };
	vector<vector<int>> meilleurs{};
	vector<vector<bool>> supp{};
	for (int i = 0; i < T(items_type); i++)
	{
		//meilleurs.push_back(MeilleurItem(items_type[i]));
		meilleurs.push_back(MeilleurItemOpti(items_type[i], panos, nb_items));
		supp.push_back(vector<bool>(T(items_type[i]), false));
	}
	//std::cout << "0\n";

	for (int i = 0; i < T(items_type); i++)
	{
		//std::cout << i << "\n";
		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (Parmi(j, meilleurs[i]))
			{
				continue;
			}
			int pos_pano{ 0 };
			if (APano(items_type[i][j], panos, pos_pano))
			{
				// on va tenter de supprimmer cet objet ! 
				//std::cout << "j = " << j << "\n";
				if (!(items_type[i][j].InfSpe(items_type[i][meilleurs[i][0]]) || items_type[i][j].InfSpe(items_type[i][meilleurs[i][1]])))
				{
					//std::cout << "continue\n";
					continue; // cet item seul n'est pas moins bien que l'item de base sans pano : il ne doit pas etre supprimé 
				}
				//std::cout << "pas continue\n";
				supp[i][j] = true;
				int m1{ meilleurs[i][2] }, m2{ meilleurs[i][3] }; 
				if (i == ANNEAU)
				{
					m1 = meilleurs[i][0];
					m2 = meilleurs[i][1];
				}
				if (nb_items[pos_pano] < 2)
				{
					if (i == ANNEAU)
					{
						supp[i][j] = (items_type[i][j].InfSpe(items_type[i][m1]) || items_type[i][j].InfSpe(items_type[i][m2]));
					}
				}
				else
				{
					// choix du 2e item de la pano 
					for (int k = 0; k < T(items_type); k++)
					{
						//std::cout << "   k = " << k << "\n";
						if (k != ANNEAU && k == i)
						{
							continue;
						}
						for (int l = 0; l < T(items_type[k]); l++)
						{
							//std::cout << "    l = " << l << "/" << T(items_type[k]) << "\n";
							if (l == j)
							{
								continue;
							}
							if (items_type[i][j].Gpano() == items_type[k][l].Gpano())
							{
								//std::cout << "item de la pano trouve\n";
								int m3{ meilleurs[k][2] }, m4{ meilleurs[k][3] };
								if (k == ANNEAU)
								{
									m3 = meilleurs[k][0];
									m4 = meilleurs[k][1];
								}
								const Cbonus paire_pano(items_type[i][j] + items_type[k][l]), paire_13(items_type[i][m1] + items_type[k][m3]), paire_14(items_type[i][m1] + items_type[k][m4]), paire_23(items_type[i][m2] + items_type[k][m3]), paire_24(items_type[i][m2] + items_type[k][m4]);
								const vector<Cbonus> bonus{ panos[pos_pano].Gtous_bonus_jusqua(nb_items[pos_pano]) };
								for (int m = 0; m < T(bonus) - 1; m++)
								{
									const Cbonus paire_b1(paire_pano + bonus[m]);
									for (int n = m + 1; n < T(bonus); n++)
									{
										const Cbonus paire_b2(paire_b1 + bonus[n]);
										if (!(paire_13.MieuxSpe(paire_b2) || paire_14.MieuxSpe(paire_b2) || paire_23.MieuxSpe(paire_b2) || paire_24.MieuxSpe(paire_b2)))
										{
											supp[i][j] = false; // ce bonus n'est pas moins bien que les paires des meilleurs items !
											break;
										}
									}
									if (!supp[i][j])
									{
										break;
									}
								}
								if (!supp[i][j])
								{
									break;
								}
							}
						}
						if (!supp[i][j])
						{
							break;
						}
					}
				}
			}
		}
	}
	bool un_supp{ false };
	int nb_supp{ 0 };
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			//std::cout << j << " : "  << supp[j] << "\n";
			if (supp[i][j])
			{
				un_supp = true;
				Supp_(items_type[i], j);
				Supp_(supp[i], j);
				j--;
				nb_supp++;
				//std::cout << nb_supp << "\n";
			}
		}
	}
	if (texte && un_supp)
	{
		std::cout << "suppression de " << nb_supp << " items, il reste : \n";
		AffItems(items_type, "");
		Pause();
	}
	else if (texte)
	{
		std::cout << "rien n'a ete suppr\n";
	}
	return un_supp;
	if (un_supp)
	{
		Clear_2(items_type, panos, texte);
	}
}

bool Clear_3(vector<vector<Citem>>& items_type, vector<Cpano> const& panos, bool texte = true)
{
	if (texte)
	{
		std::cout << "clear d'item en faisant des triplets d'items de meme pano ...\n";
	}
	vector<int> nb_items{}; // on va compter le nombre max d'item pour chaque pano 
	vector<vector<vector<int>>> pos_items{};
	ChercheItems(panos, items_type, nb_items, pos_items);
	vector<vector<int>> meilleurs{};
	vector<vector<bool>> supp{};
	for (int i = 0; i < T(items_type); i++)
	{
		//meilleurs.push_back(MeilleurItem(items_type[i]));
		meilleurs.push_back(MeilleurItemOpti(items_type[i], panos, nb_items));
		supp.push_back(vector<bool>(T(items_type[i]), false));
	}
	//std::cout << "0\n";

	for (int i = 0; i < T(panos); i++)
	{
		if (nb_items[i] < 3)
		{
			continue;
		}
		// au moins 3 items dans cette pano 
		for (int j = 0; j < T(pos_items[i]); j++)
		{
			// on essaye de supp cet item 
			const int t1{ pos_items[i][j][0] }, p1{ pos_items[i][j][1] };
			const Cbonus b1(items_type[t1][p1]);
			if (!(b1.InfSpe(items_type[t1][meilleurs[t1][0]]) || b1.InfSpe(items_type[t1][meilleurs[t1][1]])))
			{
				//std::cout << "continue\n";
				continue; // cet item seul n'est pas moins bien que l'item de base sans pano : il ne doit pas etre supprimé 
			}
			if (items_type[t1][p1].Gnom() == "Caparavent")
			{
				std::cout << "tente de suppr caparavent en triplet !!\n";
				std::cout << b1.InfSpe(items_type[t1][meilleurs[t1][0]]) << " " << b1.InfSpe(items_type[t1][meilleurs[t1][1]]) << "\n";
				std::cout << b1.Gtexte() << "\n";
				std::cout << items_type[t1][meilleurs[t1][0]].Gtexte() << "\n" << items_type[t1][meilleurs[t1][1]].Gtexte() << "\n";
				Pause();
			}
			supp[t1][p1] = true;
			int m1{ meilleurs[t1][2] }, m2{ meilleurs[t1][3] };
			if (t1 == ANNEAU)
			{
				m1 = meilleurs[t1][0];
				m2 = meilleurs[t1][1];
			}
			for (int jj = 0; jj < T(pos_items[i]); jj++)
			{
				if (j == jj)
				{
					continue;
				}
				for (int jjj = 0; jjj < T(pos_items[i]); jjj++)
				{
					if (jjj == j|| jjj == jj)
					{
						continue;
					}
					// on a les 3 items à tester ensemble ! 
					const int t2{ pos_items[i][jj][0] }, t3{ pos_items[i][jjj][0] };
					int m3{ meilleurs[t2][2] }, m4{ meilleurs[t2][3] }, m5{ meilleurs[t3][0] }, m6{ meilleurs[t3][1] };
					if (t2 == ANNEAU)
					{
						m3 = meilleurs[t2][0];
						m4 = meilleurs[t2][1];
					}
					if (t3 == ANNEAU)
					{
						m5 = meilleurs[t3][0];
						m6 = meilleurs[t3][1];
					}
					const vector<vector<int>> liste_meilleurs{ {m1,m3,m5}, {m1,m3,m6}, {m1,m4,m5}, {m1,m4,m6}, {m2,m3,m5}, {m2,m3,m6}, {m2,m4,m5}, {m2,m4,m6} };
					vector<Cbonus> meilleurs{};
					for (int k = 0; k < T(liste_meilleurs); k++)
					{
						meilleurs.push_back(items_type[t1][liste_meilleurs[k][0]] + items_type[t2][liste_meilleurs[k][1]] + items_type[t3][liste_meilleurs[k][2]]);
					}
					const vector<Cbonus> bonus{ panos[i].Gtous_bonus_jusqua(nb_items[i]) };

					const Cbonus b3(b1 + items_type[pos_items[i][jj][0]][pos_items[i][jj][1]] + items_type[pos_items[i][jjj][0]][pos_items[i][jjj][1]]);
					for (int m = 0; m < T(bonus) - 1; m++)
					{
						const Cbonus b_1(b3 + bonus[m]);
						for (int mm = m + 1; mm < T(bonus); mm++)
						{
							const Cbonus b_2(b_1 + bonus[mm]);
							for (int mmm = mm + 1; mmm < T(bonus); mmm++)
							{
								const Cbonus b_3(b_2 + bonus[mmm]);
								bool un_meilleur{ false };
								for (int n = 0; n < T(meilleurs); n++)
								{
									if (meilleurs[n].MieuxSpe(b_3))
									{
										un_meilleur = true;
										break;
									}
								}
								if (!un_meilleur)
								{
									// ce bonus n'a pas ete battu ! -> on ne doit pas supp cet item !
									supp[t1][p1] = false;
									break;
								}
							}
							if (!supp[t1][p1])
							{
								break;
							}
						}
						if (!supp[t1][p1])
						{
							break;
						}
					}
					if (!supp[t1][p1])
					{
						break;
					}

				}
				if (!supp[t1][p1])
				{
					break;
				}
			}
		}
	}
	bool un_supp{ false };
	int nb_supp{ 0 };
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			//std::cout << j << " : "  << supp[j] << "\n";
			if (supp[i][j])
			{
				un_supp = true;
				Supp_(items_type[i], j);
				Supp_(supp[i], j);
				j--;
				nb_supp++;
				//std::cout << nb_supp << "\n";
			}
		}
	}
	if (texte && un_supp)
	{
		std::cout << "suppression de " << nb_supp << " items, il reste : \n";
		AffItems(items_type, "");
		Pause();
	}
	else if (texte)
	{
		std::cout << "rien n'a ete suppr\n";
	}
	return un_supp;
	if (un_supp)
	{
		Clear_3(items_type, panos, texte);
	}
}

bool Clear_4(vector<vector<Citem>>& items_type, vector<Cpano> const& panos, bool texte = true)
{
	if (texte)
	{
		std::cout << "clear d'item en faisant des quadruplets d'items de meme pano ...\n";
	}
	vector<int> nb_items{}; // on va compter le nombre max d'item pour chaque pano 
	vector<vector<vector<int>>> pos_items{};
	ChercheItems(panos, items_type, nb_items, pos_items);
	vector<vector<int>> meilleurs{};
	vector<vector<bool>> supp{};
	for (int i = 0; i < T(items_type); i++)
	{
		//meilleurs.push_back(MeilleurItem(items_type[i]));
		meilleurs.push_back(MeilleurItemOpti(items_type[i], panos, nb_items));
		supp.push_back(vector<bool>(T(items_type[i]), false));
	}
	//std::cout << "0\n";

	for (int i = 0; i < T(panos); i++)
	{
		if (nb_items[i] < 4)
		{
			continue;
		}
		// au moins 3 items dans cette pano 
		for (int j = 0; j < T(pos_items[i]); j++)
		{
			// on essaye de supp cet item 
			const int t1{ pos_items[i][j][0] }, p1{ pos_items[i][j][1] };
			const Cbonus b1(items_type[t1][p1]);
			if (!(b1.InfSpe(items_type[t1][meilleurs[t1][0]]) || b1.InfSpe(items_type[t1][meilleurs[t1][1]])))
			{
				//std::cout << "continue\n";
				continue; // cet item seul n'est pas moins bien que l'item de base sans pano : il ne doit pas etre supprimé 
			}
			supp[t1][p1] = true;
			int m1{ meilleurs[t1][2] }, m2{ meilleurs[t1][3] };
			if (t1 == ANNEAU)
			{
				m1 = meilleurs[t1][0];
				m2 = meilleurs[t1][1];
			}
			for (int jj = 0; jj < T(pos_items[i]); jj++)
			{
				if (j == jj) // il faut voir qu'ici on autorise de mettre deux fois le meme type d'item : ca devrait pas changer grand chose 
				{
					continue;
				}
				for (int jjj = 0; jjj < T(pos_items[i]); jjj++)
				{
					if (jjj == j || jjj == jj)
					{
						continue;
					}
					for (int jjjj = 0; jjjj < T(pos_items[i]); jjjj++)
					{
						if (jjjj == j || jjjj == jj || jjjj == jjj)
						{
							continue;
						}
						// on a les 4 items à tester ensemble ! 
						const int t2{ pos_items[i][jj][0] }, t3{ pos_items[i][jjj][0] }, t4{ pos_items[i][jjjj][0] };
						int m3{ meilleurs[t2][2] }, m4{ meilleurs[t2][3] }, m5{ meilleurs[t3][0] }, m6{ meilleurs[t3][1] }, m7{ meilleurs[t4][0] }, m8{ meilleurs[t4][1] };
						if (t2 == ANNEAU)
						{
							m3 = meilleurs[t2][0];
							m4 = meilleurs[t2][1];
						}
						if (t3 == ANNEAU)
						{
							m5 = meilleurs[t3][0];
							m6 = meilleurs[t3][1];
						}
						if (t4 == ANNEAU)
						{
							m7 = meilleurs[t4][0];
							m8 = meilleurs[t4][1];
						}
						const vector<vector<int>> liste_meilleurs{ {m1,m3,m5,m7}, {m1,m3,m5,m8}, {m1,m3,m6,m7}, {m1,m3,m6,m8}, {m1,m4,m5,m7}, {m1,m4,m5,m8}, {m1,m4,m6,m7}, {m1,m4,m6,m8}, {m2,m3,m5,m7}, {m2,m3,m5,m8}, {m2,m3,m6,m7}, {m2,m3,m6,m8}, {m2,m4,m5,m7}, {m2,m4,m5,m8}, {m2,m4,m6,m7}, {m2,m4,m6,m8} };
						vector<Cbonus> meilleurs{};
						for (int k = 0; k < T(liste_meilleurs); k++)
						{
							meilleurs.push_back(items_type[t1][liste_meilleurs[k][0]] + items_type[t2][liste_meilleurs[k][1]] + items_type[t3][liste_meilleurs[k][2]] + items_type[t4][liste_meilleurs[k][3]]);
						}
						const vector<Cbonus> bonus{ panos[i].Gtous_bonus_jusqua(nb_items[i]) };

						const Cbonus b3(b1 + items_type[pos_items[i][jj][0]][pos_items[i][jj][1]] + items_type[pos_items[i][jjj][0]][pos_items[i][jjj][1]]);
						for (int m = 0; m < T(bonus) - 1; m++)
						{
							const Cbonus b_1(b3 + bonus[m]);
							for (int mm = m + 1; mm < T(bonus); mm++)
							{
								const Cbonus b_2(b_1 + bonus[mm]);
								for (int mmm = mm + 1; mmm < T(bonus); mmm++)
								{
									const Cbonus b_3(b_2 + bonus[mmm]);
									for (int mmmm = mmm + 1; mmmm < T(bonus); mmmm++)
									{
										const Cbonus b_4(b_3 + bonus[mmmm]);
										bool un_meilleur{ false };
										for (int n = 0; n < T(meilleurs); n++)
										{
											if (meilleurs[n].MieuxSpe(b_4))
											{
												un_meilleur = true;
												break;
											}
										}
										if (!un_meilleur)
										{
											// ce bonus n'a pas ete battu ! -> on ne doit pas supp cet item !
											supp[t1][p1] = false;
											break;
										}
									}
									if (!supp[t1][p1])
									{
										break;
									}
									
								}
								if (!supp[t1][p1])
								{
									break;
								}
							}
							if (!supp[t1][p1])
							{
								break;
							}
						} // premiere boucle de bonus de pano 
						if (!supp[t1][p1])
						{
							break;
						}
					}
					if (!supp[t1][p1])
					{
						break;
					}

				}
				if (!supp[t1][p1])
				{
					break;
				}
			}
		} // boucle de choix de premier item 
	}
	bool un_supp{ false };
	int nb_supp{ 0 };
	for (int i = 0; i < T(items_type); i++)
	{
		for (int j = 0; j < T(items_type[i]); j++)
		{
			//std::cout << j << " : "  << supp[j] << "\n";
			if (supp[i][j])
			{
				un_supp = true;
				Supp_(items_type[i], j);
				Supp_(supp[i], j);
				j--;
				nb_supp++;
				//std::cout << nb_supp << "\n";
			}
		}
	}
	if (texte && un_supp)
	{
		std::cout << "suppression de " << nb_supp << " items, il reste : \n";
		AffItems(items_type, "");
		Pause();
	}
	else if (texte)
	{
		std::cout << "rien n'a ete suppr\n";
	}
	return un_supp;
}

// à faire à la fin parce que ca supprime les stuff "aucun" utile pour la recuperation des meilleurs stuffs (quand il n'y a rien qui donne 1 PA) 
// supprimme : les items "aucun" choisit en meilleur par dépit 
bool ClearFinal(vector<vector<Citem>>& items_type, vector<Cpano> const& panos, bool texte = true)
{
	vector<int> nb_items{ CompteItems(panos, items_type) };

	for (int i = 0; i < T(items_type); i++)
	{
		vector<int> meilleurs{ MeilleurItemOpti(items_type[i], panos, nb_items) }; // on va quand meme faire ca pour verifier que l'objet sans pano est moins bien que le meilleur sans pano 
		// /!\ ici ca ne marche pas car l'item en position zero n'est plus aucun !! bon je pense ca doit aller quand meme vu que ya d'autre verif mais à voir 
		// et ici il faut opti pour les anneaux : un anneau de pano seul dans sa pano peut enlever une vie à un autre anneau 

		for (int j = 0; j < T(meilleurs); j++)
		{
			//std::cout << items_type[i][meilleurs[0]].Gtexte() << "\n";
			//std::cout << items_type[i][meilleurs[1]].Gtexte() << "\n";
			//Pause();
		}

		for (int j = 0; j < T(items_type[i]); j++)
		{
			if (j == meilleurs[0] || !(items_type[i][j].InfSpe(items_type[i][meilleurs[0]]) || items_type[i][j].InfSpe(items_type[i][meilleurs[1]]))) // bon là on rate quelques anneaux : à verifier ensuite 
			{
				continue;
			}
			// ici il se fait battre sans pano 

			//std::cout << "\n\n" << items_type[i][j].Gtexte() << "\n";

			vector<Cbonus> bonus{ Cbonus() };
			int pos_pano{ 0 };
			if (APano(items_type[i][j], panos, pos_pano))
			{
				bonus = panos[pos_pano].Gtous_bonus_jusqua(nb_items[pos_pano], 1); // on sait deja que sans pano il est inferieur à un objet : non ca peut etre l'objet lui-meme !
			}

			bool suppr{ true };
			for (int b = 0; b < T(bonus); b++)
			{
				bool un_supp{ true }, bonus_faible{ false };
				if (i == ANNEAU)
				{
					un_supp = false;
				}
				const Cbonus avec_pano(items_type[i][j] + bonus[b]); // le bonus à depasser 
				for (int k = 0; k < T(items_type[i]); k++)
				{
					if (k == j)
					{
						continue;
					}
					if (items_type[i][k].MieuxSpe(avec_pano))
					{
						if (texte)
						{
							std::cout << items_type[i][k].Gtexte() << " > " << items_type[i][j].Gtexte() << " + bonus de pano " << b << "\n";
						}
						if (un_supp)
						{
							bonus_faible = true;
							break;
						}
						else
						{
							un_supp = true;
						}
					}
				}
				if (!bonus_faible)
				{
					suppr = false;
					break;
				}
			}
			if (suppr)
			{
				if (texte)
				{
					std::cout << "clear final : supp " << items_type[i][j].Gnom() << "\n";
					Pause();
				}
				Supp_(items_type[i], j);
				ClearFinal(items_type, panos, texte);
				return true;
			}
		}
	}
	if (texte)
	{
		std::cout << "\nfin de clear final : il reste :\n";
		AffItems(items_type, "");
	}
	return false;
}



