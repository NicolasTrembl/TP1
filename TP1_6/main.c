/* code r�alis� par JPS en C et inspir� du code C++ de Mr. Fercoq */
#include <math.h>
#include "Station.h"
#include "Reseau.h"

#pragma region JPS_code

/// Retourne la norme d'un vecteur : utilis� par le calcule de distance entre stations
double norme(double vx, double vy)
{
    return sqrt(vx*vx + vy*vy);
}

/// La construction se fait directement depuis le fichier ouvert en cours de lecture...
Station * lecture_station (FILE * is)
{
    Station * s=(Station *)malloc(sizeof(Station));
    s->m_frequence = 0;
    fscanf(is,"%d%f%f\n",&s->m_id, &s->m_x, &s->m_y);
    return s;
}

/// M�thode de calcul de distance � une autre station (pour d�terminer le voisinage)
double distance(const Station * autre, const Station * s)
{
    return norme( autre->m_x - s->m_x, autre->m_y - s->m_y  );
}

/// Affecte une fr�quence � la station
void setFrequence(Station *s, int frequence)
{
    s->m_frequence = frequence;
}

/// Test l'affectation d'une fr�quence
/// retourne vrai si la fr�quence n'est pas en conflit avec une station adjacente
/// faux sinon
int testFrequence(Station ** stations, Station * s, int ordre, int frequence)
{
    int i=s->m_id-1;
    Station * s_adj;

    for (int j=0; j<ordre; j++)
    {
        s_adj=NULL;

        if(m_adjacents[i][j])
            s_adj=stations[j];

        if ( s_adj!=NULL && s_adj->m_frequence && s_adj->m_frequence == frequence )
            return 0;
    }

    return 1;
}

/// M�thode de d�termination des stations adjacentes ( distance < dmin )
void determineAdjacents(Station** stations, int ordre, Station *s, double dmin)
{
    for (int i=0; i<ordre; i++)
    {
        if ( distance(stations[i],s)<dmin && stations[i]!=s )
            m_adjacents[stations[i]->m_id-1][s->m_id-1]=1;
    }
}

/// La construction du r�seau se fait � partir d'un fichier
/// dont le nom est pass� en param�tre
Station ** ouvrir_reseau(char * nomFichier, int *ordre)
{
    FILE * ifs = fopen(nomFichier,"r");

    Station** stations;

    if(!ifs)
    {
        printf("Erreur de fichier");
        exit(-1);
    }

    double dmin;
    fscanf(ifs,"%lf",&dmin);

    fscanf(ifs,"%d",ordre);

    stations=(Station**) malloc(*ordre * sizeof (Station*));
    m_adjacents=(int**) malloc(*ordre * sizeof (int*));

    for(int i=0; i<*ordre; i++)
    {
        m_adjacents[i]=(int*) malloc(*ordre * sizeof (int));
        for(int j=0; j<*ordre; j++)
            m_adjacents[i][j]=0;
    }

    for (int i=0; i<*ordre; ++i)
    {
        stations[i]=lecture_station(ifs);
    }

    for (int i=0; i<*ordre; ++i)
        determineAdjacents(stations, *ordre, stations[i], dmin);

    return stations;
}

/// afficher toutes les stations du reseau
void afficher_reseau(Station **stations, int ordre)
{
    printf("Reseau d'ordre %d :\n",ordre);

    for (int i=0; i<ordre; ++i)
    {
        printf("id = %d x = %.2f y = %.2f frequence = %d",stations[i]->m_id,stations[i]->m_x,stations[i]->m_y,stations[i]->m_frequence);

        printf(" Adjacents=");

        /// Afficher les stations adjacentes � la station de num�ro m_id
        for (int j=0; j<ordre ; j++)
        {
            if (m_adjacents[stations[i]->m_id-1][j])
                printf("%d ",j+1);
        }
        printf("\n");
    }
}

/// Remet toutes les fr�quences des stations � 0 (non affect�es)
void resetFrequences(Station ** stations, int ordre)
{
    for (int i=0; i<ordre; ++i)
    {
        stations[i]->m_frequence=0;
    }
}

#pragma endregion

#pragma region etudiant_code
/// ************* CODE ETUDIANT *************

void attribuerNaif(int* nbFrequences, Station ** stations, const int ordre) {
    if (stations == NULL) return;
    if (m_adjacents == NULL) return;

    *nbFrequences = 0;
    int goalFrequences = 1;

    // parcours des sommets
    for (int i=0; i<ordre; i++) {
        while (true) {
            bool any = false;
            for (int j=0; j<ordre; j++) {
                if (m_adjacents[stations[i]->m_id-1][j]) {
                    if (stations[j]->m_frequence == goalFrequences) {
                        any = true;
                        break;
                    }
                }
            }
            if (!any) {
                break;
            }
            // si la frequence est d�j� prise par un voisin
            // on incr�mente la frequence
            goalFrequences++;
        }
        // attribution de la frequence
        stations[i]->m_frequence = goalFrequences;
        if (goalFrequences > *nbFrequences) {
            *nbFrequences = goalFrequences;
        }
        goalFrequences = 1;

    }
}

int get_degree(Station ** stations,const int ordre,const Station *s) {
    if (stations == NULL) return 0;
    if (m_adjacents == NULL) return 0;

    // r�cup�re le degr� d'un sommet
    int degrees=0;
    for (int i = 0; i < ordre; i++) {
        degrees += m_adjacents[s->m_id-1][stations[i]->m_id-1];
    }


    return degrees;
}




void attribuerWelshPowell(int* nbFrequences, Station ** stations, const int ordre) {
    if (stations == NULL) return;
    if (m_adjacents == NULL) return;


    // calcul des degrees
    int degrees[ordre] = {};
    for (int i = 0; i < ordre; i++) {
        degrees[i] = get_degree(stations, ordre, stations[i]);
    }

    // bubble sort selon les degrees
    int sorted[ordre] = {};

    for (int i = 0; i < ordre; i++) {
        sorted[i] = i;
    }

    // adapt� de :
    // https://www.geeksforgeeks.org/c-program-to-sort-an-array-in-ascending-order/
    for (int i = ordre - 2; i >= 0; i--) {
        for (int j = ordre - i - 2; j >= 0; j--) {
            if (degrees[sorted[j]] < degrees[sorted[j + 1]]) {
                const int temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    *nbFrequences = 0;
    int done = 0;
    int freq = 0;

    // Tant que tout les sommets ne sont pas color�
    while (done < ordre) {
        freq++;
        for (int i = 0; i < ordre; i++) {
            // parcours tout les sommets encore pas color�
            if (stations[sorted[i]]->m_frequence != 0) continue;
            bool touch = false;

            for (int j = 0; j < ordre; j++) {
                if (m_adjacents[sorted[i]][sorted[j]] && stations[sorted[j]]->m_frequence == freq) {
                    touch = true;
                    break;
                }
            }

            if (!touch) {
                // si le sommet n'a aucun voisin avec cette frequence
                // on lui attribu
                stations[sorted[i]]->m_frequence = freq;
                done++;
            }

        }

    }
    *nbFrequences = freq;





}
/// ************* FIN CODE ETUDIANT *************

#pragma endregion

int main()
{

    /// Chargement du r�seau et affichage avant coloration
    int ordre;
    ///LES FICHIERS SE TROUVENT DANS LE REPERTOIRE cmake-build-debug
    Station ** stations = ouvrir_reseau("reseau3.txt", &ordre);
    afficher_reseau(stations, ordre);


    /// Coloration algorithme "na�f" et affichar
    /// COMPLET�
    int nbFrequences;
    attribuerNaif(&nbFrequences, stations, ordre);
    printf("Apr�s naif : %d frequences utilisees\n", nbFrequences);
    afficher_reseau(stations, ordre);

    resetFrequences(stations, ordre);

    /// Coloration algorithme "Welsh et Powell" et affichage
    /// A COMPLETER
    int nbFrequences2;
    attribuerWelshPowell(&nbFrequences2, stations, ordre);
    printf("Apr�s Welsh Powell : %d frequences utilisees\n", nbFrequences2);
    afficher_reseau(stations, ordre);

    resetFrequences(stations, ordre);

    /// Coloration algorithme "Syst�matique" et affichage
    int nbFreqSyst;
    attribuerSystematique(&nbFreqSyst, stations, ordre);
    printf("Nombre chromatique : %d frequences utilisees (optimal)\n", nbFreqSyst);
    afficher_reseau(stations, ordre);

    // free stations

    return 0;
}

/// L'algorithme suivant trouve une colloration
/// en essayant syst�matiquement toutes les combinaisons
/// d'affectations avec 1 couleur, puis 2 couleurs, puis 3 ...
/// Cet algorithme garantit donc une meilleure coloration
/// ( nombre de couleur utilis�es = nombre chromatique )
/// mais il pr�sente un grave inconv�nient... Lequel ?
void attribuerSystematique(int* nbFrequences, Station ** stations, int ordre)
{
    *nbFrequences = 0;

    // Pour chaque nombre de fr�quences � utiliser
    while ( 1 )
    {
        ++(*nbFrequences);

        // On va tester les "nbFrequences puissance ordre" combinaisons
        // Exemple : pour r�partir 3 couleurs sur 4 sommets il y a 3^4 = 81 cas � �tudier
        // Ceci est �quivalent � un comptage en base 3 sur 4 chiffres : 0000 0001 0002 0010 0011 0012 0020 ...
        // On va tester les affectations correspondantes              : 1111 1112 1113 1121 1122 1123 1131 ...
        int nbCas = pow(*nbFrequences, ordre);
        for (int k=0; k<nbCas; ++k)
        {
            // On va tester l'affectation de nbFrequences correspondant au k �tudi�
            resetFrequences(stations, ordre);
            int nbAffecte = 0;
            int combi = k;

            // d�composition de l'entier k en base nbFrequences et affectations correspondante
            for (int i=0; i<ordre; ++i)
            {
                // R�cup�ration du chiffre suivant en base nbFrequences, +1 pour �viter le 0 (0 => pas encore de fr�quence)
                int freq = combi%(*nbFrequences) + 1;

                // Si on trouve une incompatibilit� dans l'affectation alors on va passer au k suivant
                // en sortant de la boucle d'affection des sommets du k actuel
                if ( !testFrequence(stations,stations[i],ordre, freq) )
                    break;

                // Sinon l'affectation de ce sommet est pour l'instant possible...
                setFrequence(stations[i], freq);
                ++nbAffecte;

                // Pr�paration de l'extraction du chiffre suivant en base nbFrequences
                combi /= *nbFrequences;
            }

            // Tous les sommets sont affect�s : c'est gangn� on a bien une affectation globale optimale !
            if ( nbAffecte == ordre )
                return;
        }
    }
}

