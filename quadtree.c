/*************************************************/
/*                                               */
/*                 QUADTREES                     */
/*                                               */
/* DUFOURMANTELLE Jeremy                         */
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

/*************************************************/
/*                                               */
/*                sucre syntaxique               */
/*                                               */
/*************************************************/

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then

typedef enum { FALSE, TRUE} bool;


/*************************************************/
/*                                               */
/*          definition type Image                */
/*                                               */
/*************************************************/


typedef struct Bloc_image
{   bool toutnoir;
    struct Bloc_image *fils[4];
} Bloc_image ;

typedef Bloc_image *Image ;

/* Fonctionnalite annexe */

bool est_blanche_simple(Image img){
    return img == NULL;
}

bool est_noire_simple(Image img){
    return img->toutnoir == TRUE;
}

/* 3.2 Fonctionalites a ecrire */

/* Construction d'image */

Image construit_blanc(){
    Image img = NULL;
    return img;
}

Image construit_noir(){
    Image img = (Bloc_image *) malloc (4 * sizeof(Bloc_image *) );
    img->toutnoir = TRUE;
    img->fils[0] = NULL;
    img->fils[1] = NULL;
    img->fils[2] = NULL;
    img->fils[3] = NULL;
    return img;
}

Image construit_composee(Image i1,Image i2,Image i3,Image i4){
    Image img = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
    img->toutnoir = FALSE;
    img->fils[0] = i1;
    img->fils[1] = i2;
    img->fils[2] = i3;
    img->fils[3] = i4;
    return img;
}

/* Affichage en mode normal et en profondeur */

void affichage_normal(Image img){
    if(est_blanche_simple(img)){
        printf("B");
    }
    else if (est_noire_simple(img)){
        printf("N");
    }
    else{
        printf(".");
        for(int cpt = 0;cpt <= 3; cpt++){
            if(est_blanche_simple(img->fils[cpt]) || est_noire_simple(img->fils[cpt])){
                if(est_blanche_simple(img->fils[cpt])){
                    printf("B");
                }
                else{
                    printf("N");
                }
            }
            else{
                affichage_normal(img->fils[cpt]);
            }
        }
    }
}

/* Affichage en mode profondeur */

void affichage_profondeur_aux(Image img,int profondeur){
    if(est_blanche_simple(img)){
        printf("B");
    }
    else if (est_noire_simple(img)){
        printf("N");
    }
    else{
        printf(" .%d ",profondeur);
        for(int cpt = 0;cpt <= 3; cpt++){
            if(est_blanche_simple(img->fils[cpt]) || est_noire_simple(img->fils[cpt])){
                if(est_blanche_simple(img->fils[cpt])){
                    printf("B%d",profondeur + 1);
                }
                else{
                    printf("N%d",profondeur + 1);
                }
            }
            else{
                affichage_profondeur_aux(img->fils[cpt],profondeur + 1);
            }
        }
    }
}

void affichage_profondeur(Image img){
    affichage_profondeur_aux(img,0);
}

/* FONCTION EST-NOIRE EST-BLANCHE RECURSIVE SUR LA GLOBALITE DE LIMAGE*/

bool est_noire(Image img){
    bool res = TRUE;
    if(est_blanche_simple(img)){
        res = FALSE;
    }
    else if(est_noire_simple(img)){
        res = TRUE;
    }
    else{
        int cpt = 0;
        while(res && cpt < 4){
            if(est_blanche_simple(img->fils[cpt]) || est_noire_simple(img->fils[cpt])){
                if(est_blanche_simple(img->fils[cpt])){
                    res = res && FALSE;
                }
            }
            else{
                res = res && est_noire(img->fils[cpt]);
            }
            cpt++;
        }
    }
    return res;
}

bool est_blanche(Image img){
    bool res = TRUE;
    if(est_blanche_simple(img)){
        res = TRUE;
    }
    else if(est_noire_simple(img)){
        res = FALSE;
    }
    else{
        int cpt = 0;
        while(res && cpt < 4){
            if(est_blanche_simple(img->fils[cpt]) || est_noire_simple(img->fils[cpt])){
                if(!est_blanche_simple(img->fils[cpt])){
                    res = res && FALSE;
                }
            }
            else{
                res = res && est_blanche(img->fils[cpt]);
            }
            cpt++;
        }
    }
    return res;
}

/*  FONCTION COPIE IMAGE */

Image copie(Image img){
    Image res = NULL;
    if(est_blanche_simple(img)){
        res = construit_blanc();
    }
    else if (est_noire_simple(img)){
        res = construit_noir();
    }
    else{
        res = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
        res->toutnoir = FALSE;
        for(int cpt = 0;cpt <= 3; cpt++){
            if(est_blanche_simple(img->fils[cpt]) || est_noire_simple(img->fils[cpt])){
                if(est_blanche_simple(img->fils[cpt])){
                    res->fils[cpt] = construit_blanc();
                }
                else{
                    res->fils[cpt] = construit_noir();
                }
            }
            else{
                res->fils[cpt] = copie(img->fils[cpt]);
            }
        }
    }
    return res;
}

/* CALCUL DE L'AIRE NOIRE D'UN CARRE*/

double aire_noire_aux(Image img,double currentArea){
    double area = 0.0;
    if(est_blanche_simple(img)){
        return area;
    }
    else if (est_noire_simple(img)){
        return currentArea * currentArea;
    }
    else{
        for(int cpt = 0;cpt <= 3; cpt++){
            if(est_blanche_simple(img->fils[cpt]) || est_noire_simple(img->fils[cpt])){
                if(!est_blanche_simple(img->fils[cpt])){
                    area += ((currentArea/2.0) * (currentArea/2.0));
                }
            }
            else{
                area += aire_noire_aux(img->fils[cpt],currentArea/2.0);
            }
        }
    }
    return area;
}

double aire_noire(Image img){
    return aire_noire_aux(img,1.0);
}

/* FONCTION MEME DESSIN : */

bool meme_dessin(Image i1,Image i2){
    bool res = TRUE;
    if(est_blanche_simple(i1) || est_blanche_simple(i2)){
        res = est_blanche(i1) && est_blanche(i2);
    }
    else if(est_noire_simple(i1) || est_noire_simple(i2)){
        res = est_noire(i1) && est_noire(i2);
    }
    else{
        int cpt = 0;
        while(cpt < 4 && res)
        {
            // On vÃ©rifie si une des deux images contient l'image non composee
            if(est_blanche_simple(i1->fils[cpt]) || est_blanche_simple(i2->fils[cpt])){
                res = res && est_blanche(i1->fils[cpt]) && est_blanche(i2->fils[cpt]);
            } 
            else if(est_noire_simple(i1->fils[cpt]) || est_noire_simple(i2->fils[cpt])){
                res = res && est_noire(i1->fils[cpt]) && est_noire(i2->fils[cpt]);
            }
            
            else{
                res = res && meme_dessin(i1->fils[cpt],i2->fils[cpt]);
            }
            cpt++;
        }
    }
    return res;
}

/* FONCTION DIFFERENCE */

Image difference(Image i1, Image i2){
    Image res = NULL;
    if(est_blanche_simple(i1) && est_blanche_simple(i2))
    {
        res = construit_blanc();
    }
    else if(est_noire_simple(i1) && est_noire_simple(i2))
    {
        res = construit_noir();
    }
    else if( ( est_blanche_simple(i1) || est_noire_simple(i1) ) && ( ( est_blanche_simple(i2) || est_noire_simple(i2) ) ))
    {
        res = construit_noir();
    }
    else if( NOT(est_blanche_simple(i1) || est_noire_simple(i1)) && ((est_blanche_simple(i2) || est_noire_simple(i2)) ))
    {
        res = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
        res->toutnoir = FALSE;
        for(int cpt = 0;cpt <= 3; cpt++){
            // On vÃ©rifie si une des deux images contient l'image non composee
            if(est_blanche_simple(i1->fils[cpt]) && est_blanche_simple(i2))
            {
                res->fils[cpt] = construit_blanc(); 
            } 
            else if(est_noire_simple(i1->fils[cpt]) && est_noire_simple(i2))
            {
                res->fils[cpt] = construit_noir();
            }
            else if( ( est_blanche_simple(i1->fils[cpt]) || est_noire_simple(i1->fils[cpt]) ) && ( ( est_blanche_simple(i2) || est_noire_simple(i2) ) ))
            {
                res->fils[cpt] = construit_noir();
            }
            else{
                res->fils[cpt] = difference(i1->fils[cpt],i2);
            }
        }
    }
    else if ( (est_blanche_simple(i1) || est_noire_simple(i1)) && NOT(est_blanche_simple(i2) || est_noire_simple(i2)) )
    {
        res = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
        res->toutnoir = FALSE;
        for(int cpt = 0;cpt <= 3; cpt++){
            // On vÃ©rifie si une des deux images contient l'image non composee
            if(est_blanche_simple(i1) && est_blanche_simple(i2->fils[cpt]))
            {
                res->fils[cpt] = construit_blanc(); 
            } 
            else if(est_noire_simple(i1) && est_noire_simple(i2->fils[cpt]))
            {
                res->fils[cpt] = construit_noir();
            }
            else if( ( est_blanche_simple(i1) || est_noire_simple(i1) ) && ( ( est_blanche_simple(i2->fils[cpt]) || est_noire_simple(i2->fils[cpt]) ) ))
            {
                res->fils[cpt] = construit_noir();
            }
            else{
                res->fils[cpt] = difference(i1,i2->fils[cpt]);
            }
        }
    }
    else{
        res = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
        res->toutnoir = FALSE;
        for(int cpt = 0;cpt <= 3; cpt++){
            // On vÃ©rifie si une des deux images contient l'image non composee
            if(est_blanche_simple(i1->fils[cpt]) && est_blanche_simple(i2->fils[cpt]))
            {
                res->fils[cpt] = construit_blanc(); 
            } 
            else if(est_noire_simple(i1->fils[cpt]) && est_noire_simple(i2->fils[cpt]))
            {
                res->fils[cpt] = construit_noir();
            }
            else if( ( est_blanche_simple(i1->fils[cpt]) || est_noire_simple(i1->fils[cpt]) ) && ( ( est_blanche_simple(i2->fils[cpt]) || est_noire_simple(i2->fils[cpt]) ) ))
            {
                res->fils[cpt] = construit_noir();
            }
            else{
                res->fils[cpt] = difference(i1->fils[cpt],i2->fils[cpt]);
            }
        }
    }
    return res;
}

/* fonction auxiliaire max */

int max(int a,int b)
{
    if (a > b) {return a;}
    else {return b;}
}

/* fonction auxiliaire profondeur */
int profondeur(Image img)
{
    int prof = 0;
    if(!est_blanche_simple(img) && !est_noire_simple(img))
    {
        prof++;
        int m;
        for(int cpt = 0;cpt <= 3; cpt++){
            if(cpt == 0){
                m = profondeur(img->fils[cpt]);
            }
            else{
                m = max(m,profondeur(img->fils[cpt]));
            }
        }
        prof += m;
    } 
    return prof;
}

/* PROCEDURE RENDMEMOIRE */
void rendmemoire(Image img)
{
    if(!est_blanche_simple(img)){
        if(est_noire_simple(img)){
            free(img);
        }
        else{
            for(int cpt = 0;cpt <= 3;cpt++){
                rendmemoire(img->fils[cpt]);
            }
            free(img);
        }
    }
}

/* 
    fonction complementaire stringtoimage 
    prend une chaine de caractere, un curseur sur cette chaine, init a 1
    et rend une Image
*/

Image stringtoimage(char *t,int *index)
{
    Image res = NULL;
    if(t[*index] == 'N'){
        res = construit_noir();
    }
    else if(t[*index] == 'B'){
        res = construit_blanc();
    }
    else if (t[*index] == '.'){
        res = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
        res->toutnoir = FALSE;
        for(int cpt = 0;cpt <= 3;cpt++)
        {
            *index += 1;
            //printf("index lu : %d\n",*index);
            if(t[*index] == 'N'){
                res->fils[cpt] = construit_noir();
            }
            else if(t[*index] == 'B'){
                res->fils[cpt] = construit_blanc();
            }
            else{
                res->fils[cpt] = stringtoimage(t,index);
            }
        }
    }
    return res;
}

/* fonction complementaire : qui retourne une chaine de caractere avec l'entree au clavier */
char *inputtostring(){
    char * buffer = (char *) malloc( 80 * sizeof(char));
    printf( "Veuillez saisir le codage de l'image : " );
    fflush( stdout );
    scanf( "%[^\n]", buffer );
    return buffer;
}

/* fonction complementaire de verification sur la coherence pour transformation en image */
/* verifie si l'image comporte que des caractere : N,B,. et espace*/
bool verifimagestring(char *t,int size)
{
    bool verif = TRUE;
    int cpt = 0;
    while(verif && cpt <= size-1)
    {
        if(t[cpt] != '.' && isspace(t[cpt]) && t[cpt] != 'N' && t[cpt] != 'B'){
            verif = FALSE;
        }
        cpt++;
    }
    return verif;
}

/* fonction de lecture au clavier qui demande en arguments le nombre de caractere que contient l'image */

Image codeToImage(int size){
    Image res = NULL;
    char *code = inputtostring(size);
    if(verifimagestring(code,size)){
        res = stringtoimage(code,0);
    }
    return res;
}

/* function CompteSousImagesPleines(Image,hauteur) : bool */
/*
 Verifie si une image est pleine de hauteur h
*/
bool CompteSousImagesPleines(Image img,int hauteur)
{
    bool res = TRUE;
    if(est_blanche_simple(img) || est_noire_simple(img))
    {
        res = (hauteur == 0);
    }
    else{
        int cpt = 0;
        while(cpt < 4 && res)
        {
            res = res && CompteSousImagesPleines(img->fils[cpt],hauteur - 1);
            cpt++;
        }
    }
    return res;
}

/* PROCEDURE ARRONDIT */

void arrondit(Image *img,int k)
{
    if(!est_blanche_simple(*img) && !est_noire_simple(*img))
    {
        if(k == 0){
            if(aire_noire(*img) >= 0.5)
            {
                rendmemoire(*img);
                *img = construit_noir();
            }
            else{
                rendmemoire(*img);
                *img = construit_blanc();
            }
        }
        else if(k == 1){
            for(int cpt = 0; cpt <= 3 ; cpt++)
            {
                if(aire_noire((*img)->fils[cpt]) >= 0.5)
                {
                    rendmemoire((*img)->fils[cpt]);
                    (*img)->fils[cpt] = construit_noir();
                }
                else
                {
                    rendmemoire((*img)->fils[cpt]);
                    (*img)->fils[cpt] = construit_blanc(); 
                }
            }
        }
        else{
            for(int cpt = 0; cpt <= 3 ; cpt++)
            {
                arrondit(&((*img)->fils[cpt]),k - 1);
            }
        }
        
    }
}

/* FONCTION NEGATIF */

void negatif(Image *img)
{
    //printf("tour\n");
    if(est_blanche_simple(*img))
    {
        free(*img);
        *img = construit_noir();
    }
    else if(est_noire_simple(*img))
    {
        free(*img);
        *img = construit_blanc();
    }
    else{
        for(int cpt = 0;cpt <= 3; cpt++)
        {
            negatif(&((*img)->fils[cpt]));
        }
    }
}

/* FONCTION SIMPLIFIE */

void simplifie(Image *img)
{
    if(!est_blanche_simple(*img) && !est_noire_simple(*img))
    {
        if(est_blanche(*img)){
            rendmemoire(*img);
            *img = construit_blanc();
        }
        else if(est_noire(*img)){
            rendmemoire(*img);
            *img = construit_noir();
        }
        else{
            for(int cpt = 0; cpt <= 3; cpt++)
            {
                simplifie(&((*img)->fils[cpt]));
            }
        }
    }
}

/* fonction complementaire de puissance : */
double power(double x,int n)
{
    if(n == 0){
        return 1;
    }
    else if (n == 1)
    {
        return x;
    }
    else{
        if(n % 2 == 0)
        {
            double z = power(x,n/2);
            return z * z;
        }
        else{
            double z = power(x,(n - 1)/2);
            return z * z * x;
        }
    } 
}

/* function AFFICHAGEPIXELCOMPLET : fonction non demandee */
/* Les dimensions de l'image s'adaptent a la profondeur de l'image */
/* fonction auxiliaire*/
/*
 Le principe on stocke l'image dans un tableau 2D
 puis on l'affiche
*/
void affichagepixel(Image img, int prof,int linedeb,int lineEnd,int coldeb, int colEnd,char **tab)
{
    //printf("ligne debut : %d , ligne fin : %d , colonne debut : %d , colonne fin : %d \n",linedeb,lineEnd,coldeb,colEnd);
    if(est_blanche_simple(img))
    {
        for(int l = linedeb ; l < lineEnd ; l++)
        {
            for(int c = coldeb ; c < colEnd ; c++)
            {
                tab[l][c] = '.';
            }
        }
    }
    else if(est_noire_simple(img))
    {
        for(int l = linedeb ; l < lineEnd ; l++)
        {
            for(int c = coldeb ; c < colEnd ; c++)
            {
                tab[l][c] = '8';
            }
        }
    }
    else{
        affichagepixel(img->fils[0],prof - 1, linedeb, ((linedeb + lineEnd ) / 2) ,coldeb,((coldeb + colEnd)/ 2),tab);
        affichagepixel(img->fils[1],prof - 1, linedeb , ((linedeb + lineEnd) / 2) , ((coldeb + colEnd) / 2), colEnd ,tab);
        affichagepixel(img->fils[2],prof - 1, ((linedeb + lineEnd) / 2) , lineEnd, coldeb , ((coldeb + colEnd) / 2) ,tab);
        affichagepixel(img->fils[3],prof - 1, ((linedeb + lineEnd) / 2) , lineEnd, ((coldeb + colEnd) / 2), colEnd, tab);
    }
}

/* callable function */
void affichagepixelComplet(Image img)
{
    // recuperer la profondeur de l'Image
    int prof = profondeur(img);
    // ecrire les valeurs a afficher dans un certain ordre dans un tableau
    int tailleligne = (int)power(2,prof);
    
    char **tab = (char **) malloc (tailleligne * sizeof(char *));
    
    for (int i=0; i < tailleligne; i++){
        tab[i] = (char *) malloc (tailleligne * sizeof(char));
    }

    int *cpt = (int *) malloc (sizeof(int));
    
    affichagepixel(img,prof,0,tailleligne ,0,tailleligne ,tab);

    for(int l = 0; l < tailleligne; l++)
    {
        for(int c = 0 ; c < tailleligne ; c++)
        {
            printf("%c ",tab[l][c]);
        }
        printf("\n");
    }
    printf("\n");
}

/* FONCTION AFFICHAGE2KPIXEL */
/* function auxiliaire */
void affichage2kpixel_aux(Image img, int prof,int linedeb,int lineEnd,int coldeb, int colEnd,char **tab)
{
    //printf("ligne debut : %d , ligne fin : %d , colonne debut : %d , colonne fin : %d \n",linedeb,lineEnd,coldeb,colEnd);
    if(est_blanche_simple(img))
    {
        for(int l = linedeb ; l < lineEnd ; l++)
        {
            for(int c = coldeb ; c < colEnd ; c++)
            {
                tab[l][c] = '.';
            }
        }
    }
    else if(est_noire_simple(img))
    {
        for(int l = linedeb ; l < lineEnd ; l++)
        {
            for(int c = coldeb ; c < colEnd ; c++)
            {
                tab[l][c] = '8';
            }
        }
    }
    else{
        if(prof == 0)
        {
            for(int l = linedeb ; l < lineEnd ; l++)
            {
                for(int c = coldeb ; c < colEnd ; c++)
                {
                    tab[l][c] = '-';
                }
            } 
        }
        else{
            affichage2kpixel_aux(img->fils[0],prof - 1, linedeb, ((linedeb + lineEnd ) / 2) ,coldeb,((coldeb + colEnd)/ 2),tab);
            affichage2kpixel_aux(img->fils[1],prof - 1, linedeb , ((linedeb + lineEnd) / 2) , ((coldeb + colEnd) / 2), colEnd ,tab);
            affichage2kpixel_aux(img->fils[2],prof - 1, ((linedeb + lineEnd) / 2) , lineEnd, coldeb , ((coldeb + colEnd) / 2) ,tab);
            affichage2kpixel_aux(img->fils[3],prof - 1, ((linedeb + lineEnd) / 2) , lineEnd, ((coldeb + colEnd) / 2), colEnd, tab);
        }
        
    }
}

/* Calable function */
void affichage2kpixel(Image img, int k)
{
    int prof = k;
    int tailleligne = (int)power(2,prof);
    
    char **tab = (char **) malloc (tailleligne * sizeof(char *));
    
    for (int i=0; i < tailleligne; i++){
        tab[i] = (char *) malloc (tailleligne * sizeof(char));
    }

    int *cpt = (int *) malloc (sizeof(int));
    
    affichage2kpixel_aux(img,prof,0,tailleligne ,0,tailleligne ,tab);

    for(int l = 0; l < tailleligne; l++)
    {
        for(int c = 0 ; c < tailleligne ; c++)
        {
            printf("%c ",tab[l][c]);
        }
        printf("\n");
    }
    printf("\n");
}



void affichagepc(Image img, int k){
    affichagepc_aux(img,k,FALSE,FALSE,FALSE,FALSE);
}

/* FONCTION ALEA : */
/* 
    Principe : A chaque profondeur 
    pour chaque noir de trouvant dans l'image a construire
    on le place dans un fils au hazard en utilisant une incrementation du compteur respectif
*/
Image alea(int k, int n){
    Image res = NULL;
    if(k == 0){
        if(n == 0){
            res = construit_blanc();
        }
        else{
            res = construit_noir();
        }
    }
    else{
        res = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
        res->toutnoir = FALSE;
        
        int nbelementl = power(2,k);
        int nbele = nbelementl * nbelementl;
        int elementfils = nbele/4;
        int compteur = n;
        
        if(n >= nbele)
        {
            for(int cpt = 0;cpt <= 3;cpt++)
            {
                res->fils[cpt] = alea(k - 1,elementfils);
            }
        }
        else
        {
            int fils1 = 0;
            int fils2 = 0;
            int fils3 = 0;
            int fils4 = 0;
            
            int searchedValue;
            
            for(int cpt = 0 ; cpt < n ; cpt++)
            {
                searchedValue = (rand() % 4);
                //printf("hazard : %d \n",searchedValue);
                if(searchedValue == 0 && fils1 < elementfils){
                    fils1++;
                }
                else if(searchedValue == 1 && fils2 < elementfils)
                {
                    fils2++;
                }
                else if(searchedValue == 2 && fils3 < elementfils)
                {
                    fils3++;
                }
                else if(searchedValue == 3 && fils4 < elementfils){
                    fils4++;
                }
                else{
                    cpt--;
                }   
            }
            res->fils[0] = alea(k - 1,fils1);
            res->fils[1] = alea(k - 1,fils2);
            res->fils[2] = alea(k - 1,fils3);
            res->fils[3] = alea(k - 1,fils4);
        }
    }
    return res;
}

/*
    FONCTION NEBULEUSE : 
    un peu lourd sur les appels recursifs 

    principe : 4 booleens en parametre permettent de situer l'image courante dans l'espace
    cela permet d'augmenter ou de diminuer le coef de proba de fils noir dans ces fils.

    Ce coefficient est defini pour chaque profondeur afin qu'il soit plus lourd dans les premieres
    profondeurs (proche de k) que dans celle proche de 0.

    La couleur noir est choisi a la profondeur k = 0 en fonction de la proba de tirer entre 1 et 100 
    un nombre <= coef
*/
Image nebuleuse_aux(int k,float n ,bool f1,bool f2,bool f3,bool f4)
{
    Image res = NULL;
    if(k == 0){
        int coef =(int) (n * 100);
        //printf("coef : %d \n",coef);
        //printf("coef : %d \n",coef);
        int searchedValue = (rand() % 99) + 1;
        if(searchedValue >  coef){
            res = construit_blanc();
        }
        else{
            res = construit_noir();
        }
    }
    else{
        res = (Bloc_image *) malloc (4 * sizeof(Bloc_image *));
        res->toutnoir = FALSE;
        float decrease = 1.0 / ((float) k);
        float increase = 1.0 + (1.0 - decrease);
            // Si on se trouve en haut a gauche 
            if(f1)
            {
                res->fils[0] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[1] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[2] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[3] = nebuleuse_aux(k - 1,n * increase,f1,f2,f3,f4);
            }
            // Si on se trouve en haut a droite 
            else if(f2)
            {
                res->fils[0] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[1] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[2] = nebuleuse_aux(k - 1,n * increase,f1,f2,f3,f4);
                res->fils[3] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
            }
            // Si on se trouve en bas a gauche 
            else if(f3)
            {
                res->fils[0] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[1] = nebuleuse_aux(k - 1,n * increase,f1,f2,f3,f4);
                res->fils[2] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[3] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
            }
            // Si on se trouve en bas a droite 
            else if(f4)
            {
                res->fils[0] = nebuleuse_aux(k - 1,n * increase,f1,f2,f3,f4);
                res->fils[1] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[2] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
                res->fils[3] = nebuleuse_aux(k - 1,n * decrease,f1,f2,f3,f4);
            }
            // Si c'est le tout premier appel on definit les coordonnees 
            else{
                res->fils[0] = nebuleuse_aux(k - 1,n * increase,TRUE,FALSE,FALSE,FALSE);
                res->fils[1] = nebuleuse_aux(k - 1,n * increase,FALSE,TRUE,FALSE,FALSE);
                res->fils[2] = nebuleuse_aux(k - 1,n * increase,FALSE,FALSE,TRUE,FALSE);
                res->fils[3] = nebuleuse_aux(k - 1,n * increase,FALSE,FALSE,FALSE,TRUE);
            }
    }
    return res;
}
Image nebuleuse(int k)
{
    return nebuleuse_aux(k,1,FALSE,FALSE,FALSE,FALSE);
}



/* MAIN */
int main(){

    
    printf("********************************\n");
    printf("           QUADTREE             \n");
    printf("                                \n");
    printf(" DUFOURMANTELLE JEREMY          \n");
    printf("********************************\n");


    printf("***************************************************************************\n");
    /*
        ZONE EXEMPLE
    */    
    Image exemple = construit_composee(
        construit_composee(
            construit_blanc(),
            construit_blanc(),
            construit_blanc(),
            construit_noir()
        ),
        construit_composee(
            construit_blanc(),
            construit_blanc(),
            construit_noir(),
            construit_blanc()
        ),
        construit_composee(
            construit_blanc(),
            construit_noir(),
            construit_blanc(),
            construit_blanc()
        ),
        construit_composee(
            construit_noir(),
            construit_blanc(),
            construit_blanc(),
            construit_blanc()
        )
    );
    
    // Deuxieme exemple : 
    Image exemple2 = construit_composee(
        construit_noir(),
        construit_composee(
            construit_blanc(),
            construit_blanc(),
            construit_noir(),
            construit_blanc()
        ),
        construit_blanc(),
        construit_composee(
            construit_noir(),
            construit_composee(
                construit_noir(),
                construit_noir(),
                construit_blanc(),
                construit_composee(
                    construit_noir(),
                    construit_blanc(),
                    construit_noir(),
                    construit_noir()
                )
            ),
            construit_blanc(),
            construit_noir()
        )
    );
    
    Image exempleAire = construit_composee(
        construit_noir(),
        construit_blanc(),
        construit_noir(),
        construit_composee(
            construit_noir(),
            construit_blanc(),
            construit_noir(),    
            construit_blanc()
        )
    );
    
    
    Image imgmmd1 = construit_composee(
                        construit_composee( 
                                construit_noir(), 
                                construit_noir(),
                                construit_noir(),
                                construit_composee( 
                                    construit_noir(),
                                    construit_noir(),
                                    construit_noir(),
                                    construit_composee( 
                                        construit_noir(),
                                        construit_noir(),
                                        construit_noir(),
                                        construit_noir()
                                    )
                                )
                        ), 
                        construit_blanc(), 
                        construit_noir(), 
                        construit_composee(
                            construit_blanc(),
                            construit_composee(
                                construit_blanc(),
                                construit_blanc(),
                                construit_blanc(),
                                construit_blanc()
                            ),
                            construit_blanc(),
                            construit_noir()
                        )
                    );
    Image imgmmd2 = construit_composee(
                    construit_noir(),
                    construit_blanc(),
                    construit_noir(),
                    construit_composee( 
                        construit_blanc(),
                        construit_blanc(),
                        construit_blanc(),
                        construit_composee( 
                            construit_noir(),
                            construit_noir(),
                            construit_noir(),
                            construit_noir()
                        )
                    )
                );
    
    Image ttNoir = construit_composee(
        construit_noir(),
        construit_noir(),
        construit_noir(),
        construit_composee(
            construit_noir(),
            construit_noir(),
            construit_noir(),
            construit_composee(
                construit_noir(),
                construit_noir(),
                construit_noir(),
                construit_noir()
                )
            )
        ) ;  
     
    Image img = construit_composee(construit_noir(),construit_noir(),construit_noir(),construit_noir());

    /*
        FIN ZONE EXEMPLE
    */

    /* fonction d'affichage normal */
    printf("\nFonction de construction dispo dans le code.\n");
    printf("-------------------------------------------------------------\n");
    
    printf("\nFonction d'affichage Normal :\n");
    printf("\nAffichage_normal(e1) : \n");
    affichage_normal(exemple);
    printf("\n");
    printf("\nAffichage_normal(e2) : \n");
    affichage_normal(exemple2);
    printf("\n");

    printf("----------------------------------------------------------------\n");
    
    printf("\nFonction d'affichage en profondeur :\n");
    /* fonction d'affichage en profondeur */
    printf("\nAffichage_profondeur(e2) : \n");
    affichage_profondeur(exemple2);
    printf("\n");
    
    printf("----------------------------------------------------------------\n");
    
    /* condition  qui renvoient vrai si l'image n'est pas toute blanche ni toute noire*/
    printf("Est-noire(e2) / Est-blanche(e2)\n");
    printf("On teste si l'image n'est pas toute blanche ni toute noire.\n");
    if(!(est_noire(exemple2) || est_blanche(exemple2))){
        printf("\nL'image exemple2 n'est pas toute blanche ou n'est pas toute noire.\n");
    }
    else{
        printf("\nL'image exemple2 est toute blanche ou est toute noire.\n");
    }
    printf("\n");
    printf("----------------------------------------------------------------\n");
    
    /* fonction qui copie l'image */
    printf("\naffichage_profondeur(Copie(e2)) : \n ");
    affichage_profondeur(copie(exemple2));
    printf("\n");
    printf("----------------------------------------------------------------\n");
    
    printf("\naire_noire(");
    /* fonction qui calcul l'aire noire d'une image */
    affichage_normal(exempleAire);
    double d = aire_noire(exempleAire);
    printf(") = %f\n",d);
    printf("\n");
    
    printf("----------------------------------------------------------------\n");

    /* fonction de meme dessin pour savoir si deux images sont identiques visuellement */
    printf("Algo de meme dessin : \n");
    printf("Meme-dessins(");
    affichage_normal(imgmmd1);
    printf(" , ");
    affichage_normal(imgmmd2);
    printf(")\n");
    //printf("----------------------------------------------------------------\n");
    if(meme_dessin(imgmmd1,imgmmd2)){
        printf("=> Ce sont les memes dessins !\n");
    }
    else{
        printf("=> Ce ne sont pas les memes dessins !\n");
    }
    printf("----------------------------------------------------------------\n");
    /* fonction qui rend la difference entre deux images*/
    printf("\n");
    printf("Affichage des differences de deux images : \n");
    printf("Difference(");
    affichage_normal(imgmmd1);
    printf(" , ");
    affichage_normal(imgmmd2);
    printf(")\n=> ");
    affichage_normal(difference(imgmmd1,imgmmd2));
    printf("\n");
    printf("----------------------------------------------------------------\n");
    /* fonction qui rend la profondeur max d'une image */
    printf("\nFONCTION QUI REND LA PROFONDEUR MAX (Non demandee) \n");
    printf("profondeur(");
    affichage_normal(imgmmd1);
    printf(") = ");
    printf("%d \n ",profondeur(imgmmd1));
    printf("\n");
    printf("----------------------------------------------------------------\n");
    /* fonction qui rend a la memoire*/
    printf("rendmemoire(");
    affichage_normal(imgmmd1);
    printf(")\n");
    rendmemoire(imgmmd1);
    printf("----------------------------------------------------------------\n");
    /* 
        A de-commenter pour utilisation de la fonction : 
        fonction de creation d'image par lecture au clavier
    */
     
    /*
    char *str = inputtostring();
    printf("\n");
    if(verifimagestring(str,80)){
        printf("le codage de l'image ne comporte pas d'erreurs \n");
    }
    else{
        printf("le codage de l'image comporte certaines erreurs \n");
    }
    int *cpt = (int *)malloc(sizeof(int));
    *cpt = 0;
    Image resulttostring = stringtoimage(str,cpt);
    //codeToImage(80); (en travaux pour une version plus simplifiee)
    affichage_normal(resulttostring);
    printf("\n");
    */


    /* fonction pour compter si l'image est pleine avec une certaine hauteur*/
    printf("----------------------------------------------------------------\n");
    printf("fonction pour verifier si une image est pleine de hauteur h\n");
    int *cpt = (int *)malloc(sizeof(int));
    *cpt = 0;
    Image resulttostring = stringtoimage("..BBNB.NNBN.BBBN.NNNB",cpt);
    printf("CompteSousImagesPleines(..BBNB.NNBN.BBBN.NNNB,2) \n");
    if(CompteSousImagesPleines(resulttostring,2))
    {
        printf("=> L'image est pleine de hauteur 2\n");
    }
    else{
        printf("=> L'image n'est pas pleine de hauteur 2\n");
    }
    printf("\n");
    printf("----------------------------------------------------------------\n");

    /* fonction qui rend une image arrondit avec une profondeur donnee */
    printf("fonction qui rend une image arrondit avec une profondeur donnÃ©e\n");
    int *cpt2 = (int *)malloc(sizeof(int));
    *cpt2 = 0;
    Image *aArrondir = (Image *) malloc(sizeof(Image));
    *aArrondir = stringtoimage(".N.BBNBB.N.NNB.NBNNBN",cpt2);
    arrondit(aArrondir,2);

    printf("Arrondir(.N.BBNBB.N.NNB.NBNNBN,2) = ");
    affichage_normal(*aArrondir);
    printf("\n");
    printf("----------------------------------------------------------------\n");
    
    /* fonction qui renvoit le negatif d'une image  : */
    
    printf("fonction qui renvoit le negatif d'une image\n");

    int *cpt3 = (int *)malloc(sizeof(int));
    *cpt3 = 0;
    Image *neg = (Image *) malloc(sizeof(Image));
    *neg = stringtoimage(".N.BBNBB.N.NNB.NBNNBN",cpt3);
    negatif(neg);
    printf("Negatif(.N.BBNBB.N.NNB.NBNNBN) = ");
    affichage_normal(*neg);
    printf("\n");
    printf("----------------------------------------------------------------\n");

    printf("fonction qui simplifie une image\n");
    /* fonction qui simplifie une image */
    int *cpt4 = (int *) malloc(sizeof(int));
    *cpt4 = 0;
    Image *simp = (Image *) malloc(sizeof(Image));
    *simp = construit_composee(
                construit_composee(
                    construit_noir(),
                    construit_noir(),
                    construit_noir(),
                    construit_noir()
                ),
                construit_composee(
                    construit_blanc(),
                    construit_blanc(),
                    construit_blanc(),
                    construit_blanc()
                ),
                construit_composee(
                    construit_noir(),
                    construit_noir(),
                    construit_noir(),
                    construit_noir()
                ),
                construit_noir()
            );
    stringtoimage("..NN.NNNNN.BBBBN.BNBN",cpt4);
    simplifie(simp);
    printf("resultat de simplifie(..NN.NNNNN.BBBBN.BNBN) = ");
    affichage_normal(*simp);
    printf("\n");
    printf("----------------------------------------------------------------\n");

    Image *exemplepixel = (Image *) malloc(sizeof(Image));
    int *cpt5 = (int *) malloc(sizeof(int));
    *cpt5 = 0;
    *exemplepixel = stringtoimage(".N.BBNBB.N.NNB.NBNNBN",cpt5);

    /* fonction d'affichage en pixel complet*/
    
    printf("Affichage en pixel complet de : .N.BBNBB.N.NNB.NBNNBN  \n");
    printf("But : afficher une image avec la bonne resolution )\n");
    affichagepixelComplet(*exemplepixel);
    
    /* fonction d'affichage en 2k pixel*/
    
    affichage2kpixel(*exemplepixel,3);
    

    /* fonction de generation d'une image aleatoire avec une profondeur et un nombre d'image noires*/
    printf("Affichage d'une image aleatoire(58) de profondeur 5 :\n");
    srand(time(NULL));
    Image *al = (Image *) malloc(sizeof(Image));
    *al = alea(5,58);
    affichage2kpixel(*al,5);
    
    /* fonction de generation d'une image en forme de nebuleuse avec une profondeur defini*/
    printf("Affichage d'une nebuleuse de profondeur 6 :\n");
    srand(time(NULL));
    Image *nebul = (Image *) malloc(sizeof(Image));
    *nebul = nebuleuse(6);
    affichage2kpixel(*nebul,6);
    return 0;
}
