#include <cv.h>
#include <map>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <time.h>
using namespace cv;
using namespace std;
/**
  ELIODOR Ednalson Guy Mirlin
 Promo 21 IFI
 2016-2017
                                  
              +++++++++++++++++++++++++++++++++++++++++
              -----------------------------------------
   Ce code est 100% écrit par nous meme et ne doit etre utilisé qu'à des fins académiques
              -----------------------------------------
              +++++++++++++++++++++++++++++++++++++++
**/
//

#define RACINE "test/"
#define PRECISION 0.0001
//distance euclidienne d'une individu à un autre
typedef struct VecteurClasse VecteurClasse;
typedef struct Centre Centre;
struct VecteurClasse
{
    int classe;
    float * vecteur;
};

struct Centre
{
    float * vecteur;
};
float dist_histo_couleur(float* individu1, float* individu2, int taille)
{
    float somme=0.0;
    float distance = 0.0;
    for (int i = 0; i < 3*taille; i++)
    {
        somme += individu1[i];
    }
    for (int i = 0; i < 3*taille; i++)
    {
        distance += abs((individu1[i] - individu2[i]));
    }
    if (somme==0) return 0;
    else
        return distance/somme;
}
int getMinImageGris(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int min=255;
    Scalar pixel ;
    int g ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<uchar>(i,j);

            g = (int)pixel.val[0];

            if(g<min)
            {
                min=g;
            }
        }
    }
    return min;


}

int getMaxImageGris(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int max=0;
    Scalar pixel ;
    int g ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<uchar>(i,j);

            g = (int)pixel.val[0];

            if(g>max)
            {
                max=g;
            }
        }
    }
    return max;


}
//chercher les tonalités max  et min d'une image vert rouge et bleue
int getMaxImageVert(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int max=0;
    Vec3b pixel ;
    int g ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<Vec3b>(i,j);

            g = (int)pixel[1];

            if(g>max)
            {
                max=g;
            }
        }
    }
    return max;
}
int getMaxImageRouge(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int max=0;
    Vec3b pixel ;
    int r ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<Vec3b>(i,j);

            r= (int)pixel[2];

            if(r>max)
            {
                max=r;
            }
        }
    }
    return max;
}
int getMaxImageBleue(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int max=0;
    Vec3b pixel ;
    int b ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<Vec3b>(i,j);

            b= (int)pixel[0];

            if(b>max)
            {
                max=b;
            }
        }
    }
    return max;
}

int getMinImageVert(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int min=255;
    Vec3b pixel ;
    int g ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<Vec3b>(i,j);

            g = (int)pixel[1];

            if(g<min)
            {
                min=g;
            }
        }
    }
    return min;
}
int getMinImageRouge(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int min=255;
    Vec3b pixel ;
    int r ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<Vec3b>(i,j);

            r= (int)pixel[2];

            if(r<min)
            {
                min=r;
            }
        }
    }
    return min;
}
int getMinImageBleue(Mat image)
{
    int hauteur=image.rows;
    int largeur=image.cols;
    int min=255;
    Vec3b pixel ;
    int b ;


    for(int i= 0; i <hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel = image.at<Vec3b>(i,j);

            b= (int)pixel[0];

            if(b<min)
            {
                min=b;
            }
        }
    }
    return min;
}






//Cette fonction permet de déssiner une ligne dans une image quittant d'un point start à un point end
//thickness mesure l'epaisseur de la ligne et Scalar(0,0,0) donne la couleur de celle-ci
void ligneGris( Mat img, Point start, Point end,int niveauGris,int thickness)
{

    int lineType = 8;
    line( img,
          start,
          end,
          Scalar(niveauGris),
          thickness,
          lineType );
}
//Cette fonction permet de déssiner une ligne dans une image quittant d'un point start à un point end
//thickness mesure l'epaisseur de la ligne et Scalar(0,0,0) donne la couleur de celle-ci
void ligne( Mat img, Point start, Point end,int rouge, int vert, int  bleue,int thickness)
{

    int lineType = 8;
    line( img,
          start,
          end,
          Scalar( rouge, vert, bleue ),
          thickness,
          lineType );
}

void histogrammeCouleurGris(Mat image, char * imageName)
{
//image de travail utilisée pour calculer l'histogramme pour une image en niveau de gris

    //je cherche les points situés aux extrémités de l'image en abscisse et au milieu en ordonnées
    int hauteur=image.rows;
    int largeur=image.cols;
    Mat imageHistoGris(256,256, CV_8UC3, Scalar(255,255,255));

    //déclaration des points utiles pour le dessin du profil d'intensité et initialisation de départ
    Point p0g,p1g;
    Vec3b pixel ;
    int g ;
    int maxg;
    // allcoate memory for no of pixels for each intensity value
    int histogram[256];

    // initialize all intensity values to 0
    for(int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }

    // calculate the no of pixels for each intensity values
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            histogram[(int)(image.at<Vec3b>(y,x)[0])]++;

// je cherche le maximum de l'image pour pouvoir normaliser selon la hauteur de l'image
    maxg = histogram[0];
    for(int i = 1; i < 256; i++)
    {
        if(maxg < histogram[i])
        {
            maxg = histogram[i];
        }
    }
    // cout << maxg << "\n";
    // maintenant je parcours toute la ligne restante pour en extraire l'information sur le niveau de gris
    for(int k=0; k<256; k++)
    {


        p0g=Point(k,imageHistoGris.rows);
        p1g=Point(k,imageHistoGris.rows-histogram[k]*imageHistoGris.rows/maxg);
        ligne(imageHistoGris,p0g,p1g,30,30,30,1);

    }
    namedWindow(imageName,CV_WINDOW_AUTOSIZE);
    imshow(imageName,image);
    //je fabrique les matrices et les fenêtres qui les contiendrons
    char imageNameHistoGris[160]="histogris_";
    strcat(imageNameHistoGris,imageName);
    namedWindow(imageNameHistoGris,CV_WINDOW_AUTOSIZE);

    //afficher les profils dans les fenêtres
    imshow(imageNameHistoGris,imageHistoGris);

    //Enregistrer les profils sous formes de fichier image
    imwrite(imageNameHistoGris,imageHistoGris);


}






//afficher l'histogramme d'une image en niveau de gris
//histogramme pour une image  couleur

void histogrammeCouleur(Mat image, char * imageName)
{
//image de travail utilisée pour calculer le profil d'intensité je fais une copie avant de faire le tracé sur l'autre image afin de ne 	pas troncer les résultats

    //je cherche les points situés aux extrémités de l'image en abscisse et au milieu en ordonnées
    int hauteur=image.rows;
    int largeur=image.cols;

    // je cherche un pointeur pour la ligne du milieu
    int countg,countr,countb,maxg,maxr,maxb;
    //déclaration des points utiles pour le dessin du profil d'intensité et initialisation de départ
    Point p0r,p0g,p0b,p1r,p1g,p1b;
    Vec3b pixel ;
    Mat imageHistoRouge(256,256, CV_8UC3, Scalar(255,255,255));
    Mat imageHistoVert(256,256, CV_8UC3, Scalar(255,255,255));
    Mat imageHistoBleue(256,256, CV_8UC3, Scalar(255,255,255));
    // allcoate memory for no of pixels for each intensity value
    int histogramr[256];
    int histogramg[256];
    int histogramb[256];
    // initialize all intensity values to 0
    for(int i = 0; i < 256; i++)
    {
        histogramr[i] = 0;
        histogramg[i] = 0;
        histogramb[i] = 0;
    }
    int bleue,vert,rouge;
    // calculate the no of pixels for each intensity values
    for(int y = 0; y < image.rows; y++)
    {
        for(int x = 0; x < image.cols; x++)
        {

            histogramb[(int)(image.at<Vec3b>(y,x)[0])]++;
            histogramg[(int)(image.at<Vec3b>(y,x)[1])]++;
            histogramr[(int)(image.at<Vec3b>(y,x)[2])]++;

        }
    }
    maxr=histogramr[0];
    maxb=histogramb[0];
    maxg=histogramg[0];
// je cherche le maximum de l'image pour pouvoir normaliser selon la hauteur de l'image
    for(int i = 1; i < 256; i++)
    {
        if(maxg < histogramg[i])
        {
            maxg = histogramg[i];
        }
        if(maxr < histogramr[i])
        {
            maxr = histogramr[i];
        }
        if(maxb < histogramb[i])
        {
            maxb = histogramb[i];
        }

    }

// maintenant je parcours toute la ligne restante pour en extraire l'information sur le niveau de gris
    for(int k=0; k<256; k++)
    {

        p0b=Point(k,imageHistoBleue.rows);
        p1b=Point(k,imageHistoBleue.rows-histogramb[k]*imageHistoBleue.rows/maxb);
        p0g=Point(k,imageHistoVert.rows);
        p1g=Point(k,imageHistoVert.rows-histogramg[k]*imageHistoVert.rows/maxg);
        p0r=Point(k,imageHistoRouge.rows);
        p1r=Point(k,imageHistoRouge.rows-histogramr[k]*imageHistoRouge.rows/maxr);
        ligne(imageHistoBleue,p0b,p1b,100,0,0,1);
        ligne(imageHistoVert,p0g,p1g,0,100,0,1);
        ligne(imageHistoRouge,p0r,p1r,0,0,100,1);
    }
    namedWindow(imageName,CV_WINDOW_AUTOSIZE);
    imshow(imageName,image);
    //je fabrique les matrices et les fenêtres qui les contiendrons
    char imageNameHistoBleue[80]="histobleue_";
    strcat(imageNameHistoBleue,imageName);
    namedWindow(imageNameHistoBleue,CV_WINDOW_AUTOSIZE);
    char imageNameHistoVert[80]="histovert_";
    strcat(imageNameHistoVert,imageName);
    namedWindow(imageNameHistoVert,CV_WINDOW_AUTOSIZE);
    char  imageNameHistoRouge[160]="historouge_";
    strcat(imageNameHistoRouge,imageName);
    namedWindow(imageNameHistoRouge,CV_WINDOW_AUTOSIZE);

    //afficher les profils dans les fenêtres
    imshow(imageNameHistoRouge,imageHistoRouge);
    imshow(imageNameHistoVert,imageHistoVert);
    imshow(imageNameHistoBleue,imageHistoBleue);
    //Enregistrer les profils sous formes de fichier image
    /* imwrite(imageNameHistoRouge,imageHistoRouge);
     imwrite(imageNameHistoVert,imageHistoVert);
     imwrite(imageNameHistoBleue,imageHistoBleue);*/
}

//afficher l'histogramme d'une image en niveau de gris
//histogramme pour une image  couleur

float* histogrammeCouleurEchantillon(Mat image, char * imageName, int nbreNiveaux)
{
//image de travail utilisée pour calculer le profil d'intensité je fais une copie avant de faire le tracé sur l'autre image afin de ne 	pas troncer les résultats

    //je cherche les points situés aux extrémités de l'image en abscisse et au milieu en ordonnées
    int hauteur=image.rows;
    int largeur=image.cols;

    // je cherche un pointeur pour la ligne du milieu
    int maxg,maxr,maxb;
    //déclaration des points utiles pour le dessin du profil d'intensité et initialisation de départ
    Point p0r,p0g,p0b,p1r,p1g,p1b;
    Vec3b pixel ;
    //Mat imageHistoRouge(256,256, CV_8UC3, Scalar(255,255,255));
    //Mat imageHistoVert(256,256, CV_8UC3, Scalar(255,255,255));
    //Mat imageHistoBleue(256,256, CV_8UC3, Scalar(255,255,255));
    // allcoate memory for no of pixels for each intensity value
    int histogramr[nbreNiveaux];
    int histogramg[nbreNiveaux];
    int histogramb[nbreNiveaux];
    // initialize all intensity values to 0
    for(int i = 0; i < nbreNiveaux; i++)
    {
        histogramr[i] = 0;
        histogramg[i] = 0;
        histogramb[i] = 0;
    }
    //cout << "bal lab" << nbreNiveaux<<"\n";
    int segment=256/nbreNiveaux;
    //  cout << segment << "\n";
    // calculate th no of pixels for each intensity values
    for(int y = 0; y < image.rows; y++)
    {
        for(int x = 0; x < image.cols; x++)
        {
            int i1=1;
            bool trouve1,trouve2,trouve3;
            trouve1=false;
            trouve2=false;
            trouve3=false;
            while((!trouve1)&&(i1<= nbreNiveaux))
            {
                if(((i1-1)*segment<=(int)(image.at<Vec3b>(y,x)[0]))&&(i1*segment>(int)(image.at<Vec3b>(y,x)[0])))
                {
                    histogramb[(i1-1)]++;
                    trouve1=true;
                    //cout <<"true" << i1 <<"\n";
                }
                //cout <<"i = " << i1 << "pixel = " <<(int)image.at<Vec3b>(y,x)[0]<< "\n";
                i1++;
            }

            int i2=1;
            while((!trouve2)&&(i2<= nbreNiveaux))
            {
                if(((i2-1)*segment<=(int)(image.at<Vec3b>(y,x)[1]))&&(i2*segment>(int)(image.at<Vec3b>(y,x)[1])))
                {
                    histogramg[(i2-1)]++;
                    trouve2=true;
                    //   cout <<"true" << i2 <<"\n";
                }
                i2++;
            }


            int i3=1;
            while((!trouve3)&&(i3<= nbreNiveaux))
            {
                if(((i3-1)*segment<=(int)(image.at<Vec3b>(y,x)[2]))&&(i3*segment>(int)(image.at<Vec3b>(y,x)[2])))
                {
                    histogramr[(i3-1)]++;
                    trouve3=true;
                    // cout <<"true" << i3 <<"\n";
                }
                // cout <<"i3 = " << i3 << "pixel = " <<(int)image.at<Vec3b>(y,x)[2]<< "\n";
                i3++;
            }

        }
    }
    maxr=histogramr[0];
    maxb=histogramb[0];
    maxg=histogramg[0];

// je cherche le maximum de l'image pour pouvoir normaliser selon la hauteur de l'image
    for(int i = 1; i < nbreNiveaux; i++)
    {
        if(maxr < histogramr[i])
        {
            maxr = histogramr[i];
        }
        if(maxg < histogramg[i])
        {
            maxg = histogramg[i];
        }

        if(maxb < histogramb[i])
        {
            maxb = histogramb[i];
        }


    }



    float* vecteur=new float[3*nbreNiveaux];
    for(int i=0; i<3*nbreNiveaux; i++)
    {
        if(i<nbreNiveaux)
            vecteur[i]=histogramb[i];
        else if((i>=nbreNiveaux)&&(i<2*nbreNiveaux))
        {

            vecteur[i]=histogramg[i-nbreNiveaux];
        }
        else
        {
            vecteur[i]=histogramr[i-2*nbreNiveaux];
        }

    }
    return vecteur;
}

void histogrammeGris(Mat image, char * imageName)
{
// pour les images en niveau de gris
    int largeur=image.cols;
    int hauteur=image.rows;
    int maxg;
    namedWindow(imageName,CV_WINDOW_AUTOSIZE);
    imshow(imageName,image);
    //je fabrique les matrices et les fenêtres qui les contiendrons
    char imageNameHistoGris[80]="histoGris_";
    strcat(imageNameHistoGris,imageName);
    namedWindow(imageNameHistoGris,CV_WINDOW_AUTOSIZE);
    Mat imageHistoGris(255,255, CV_8UC1, Scalar(255));
    Scalar pixel;
    int count=0;
    maxg=getMaxImageGris(image);
    //déclaration des points utiles pour le dessin de l'histogramme
    Point p0g,p1g;



    // allcoate memory for no of pixels for each intensity value
    int histogram[256];

    // initialize all intensity values to 0
    for(int i = 0; i < 255; i++)
    {
        histogram[i] = 0;
    }

    // calcule le nombre de pixel pour chaque niveau d'intensité
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y,x)]++;

// je cherche le maximum de l'image pour pouvoir normaliser selon la hauteur de l'image
    maxg = histogram[0];
    for(int i = 1; i < 256; i++)
    {
        if(maxg < histogram[i])
        {
            maxg = histogram[i];
        }
    }
    // maintenant je parcours toute la ligne restante pour en extraire l'information sur le niveau de gris
    for(int k=0; k<256; k++)
    {


        p0g=Point(k,255);
        p1g=Point(k,imageHistoGris.rows-(histogram[k]*imageHistoGris.rows)/(maxg));


        ligne(imageHistoGris,p0g,p1g,20,20,20,1);

    }

    //afficher le profil dans le fenêtre
    imshow(imageNameHistoGris,imageHistoGris);
    //Enregistrer le profil sous forme de fichier image
    imwrite(imageNameHistoGris,imageHistoGris);
}


//teste si une image a trois channels est grise
int grise(Mat img)
{
    //pour les images avec 3 channels
    Vec3b pixel;
    int r,g,b;
// Je teste le type d'image la couleur
    int largeur=img.cols;
    int hauteur=img.rows;
    // je parcours tous les pixels de l'image pour savoir si elle est grise
    for(int i = 0; i < hauteur; i++)
    {
        for(int j = 0; j < largeur; j++)
        {

            pixel =  img.at<Vec3b>(i,j);
            r = (int)pixel[2];
            g = (int)pixel[1];
            b = (int)pixel[0];
            if((r==g)&&(g==b))
            {


            }
            else
            {
                return -1;

            }
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    string aide="Erreur";

    int nbreNiveaux=atoi(argv[1]);
    int nbreCluster=atoi(argv[2]);

    /* initialize random seed: */
    srand (time(NULL));

    if (argc==3)
    {


        //pour les images avec 3 channels
        Vec3b pixel;
        int r,g,b;
// Je teste le type d'image la couleur

        //float * vecteurFichier=histogrammeCouleurEchantillon(image,imageName,nbreNiveaux);
        //Créer les centres de manière aléatoire un centre est un vecteur
        Centre centres[nbreCluster]={};
        Centre anciencentres[nbreCluster]={};

        //histogrammeCouleurEchantillon(image1,imageName1,nbreNiveaux);
        vector<cv::String> fn;
        glob("test/*.png", fn, false);

        vector<Mat> images;
        vector<VecteurClasse> imagesClasses;
        //vector<float> distances;
        size_t count = fn.size(); //number of png files in images folder
        // cout  <<  " test " <<count << "\n";

        for (size_t i=0; i<count; i++)
        {


                images.push_back(imread(fn[i]));

                //vecteur descripteur
                float* vecteurBase= histogrammeCouleurEchantillon(images[i],const_cast<char*>(fn[i].c_str()),nbreNiveaux) ;
                //initialisation de tous les individus  à la classe 0 dès le départ
                VecteurClasse vc;
                // v.push_back(vc);
                vc.vecteur=vecteurBase;
                //mise de l'individu dans la liste de tous les individus
                imagesClasses.push_back(vc);



        }
        vector<int> tabindice;
//choix initial de nbreCluster centres aléatoirement dans la liste
        for(int i=0; i<nbreCluster; i++)
        {
            bool trouve=false;

            int indicecentre;
            while(!trouve)
            {
                indicecentre=rand() % (count-1);

                if(std::find(tabindice.begin(), tabindice.end(), indicecentre) != tabindice.end())
                {

                }
                else
                {
                    tabindice.push_back(indicecentre);
                    trouve=true;
                }


            }
            //imagesClasses[indicecentre].classe=indicecentre;
            centres[i].vecteur=imagesClasses[indicecentre].vecteur;
            anciencentres[i].vecteur= new float[3*nbreNiveaux];
            //nouveaucentres.push_back(imagesClasses[indicecentre]);
           // v.push_back(imagesClasses[indicecentre]);

           // cout << "indice centres " << indicecentre  << "\n";
        }
        /*for(int k=0; k<3*nbreNiveaux; k++)
        {
            cout <<"centre"<<centres[0].vecteur[k]<<"\n";
        }*/
        bool erreur=false;
        // while(!erreur)
        //{
//calcul de la distance entre chaque vecteur image et chacun des centres puis prendre la classe du centre pour l'affecter au vecteur image en question
        while(!erreur)
        {
            for(int i=0; i<imagesClasses.size(); i++)
            {
                int nouvelleclasse;
                double distance=100;
                for(int j=0; j<nbreCluster; j++)
                {
                    //cout << "distance "<<dist_histo_couleur(imagesClasses[i].vecteur,centres[j].vecteur,nbreNiveaux)<<"\n";
                    if(dist_histo_couleur(imagesClasses[i].vecteur,centres[j].vecteur,nbreNiveaux)<distance)
                    {

                        imagesClasses[i].classe=j;
                        distance=dist_histo_couleur(imagesClasses[i].vecteur,centres[j].vecteur,nbreNiveaux);

                    }

                }

            }

//recalculer les nouveaux centres
//1-initialiser les centres à 0
            for(int j=0; j<nbreCluster; j++)
            {
                for(int k=0; k<3*nbreNiveaux; k++)
                {
                    centres[j].vecteur[k]=0;
                    //v[j].vecteur[k]=0;
                }

            }

//2-parcours des différentes images pour calculer les centres de gravité calculer l'effectif de chaque clusters puis faire la somme sur chaque composante de centre en cas de correspondance
            int compteur[nbreCluster]= {};

            // cout <<"taille " << imagesClasses.size() <<"\n";
            for(int i=0; i<imagesClasses.size(); i++)
            {
                //cout << "classe " << imagesClasses[i].classe << "\n";
                compteur[imagesClasses[i].classe]++;
                //cout << "compteur " << compteur[imagesClasses[i].classe];
                for(int k=0; k<3*nbreNiveaux; k++)
                {
                    centres[imagesClasses[i].classe].vecteur[k]+=imagesClasses[i].vecteur[k];
                    // v[imagesClasses[i].classe].vecteur[k]+=pow(imagesClasses[i].vecteur[k],2);
                }

            }
           /* for(int i=0;i<nbreCluster;i++){
                cout << " cluster" << i <<"= "<< compteur[i] << "\n";
            }*/



//3-pour chaque centre, faire la moyenne des points de sa classe

            for(int j=0; j<nbreCluster; j++)
            {
                for(int k=0; k<3*nbreNiveaux; k++)
                {
                    if(compteur[j]!=0)
                    {
                    centres[j].vecteur[k]/=(compteur[j]);
                    // v[j].vecteur[k]/=(compteur[j]);
                    //v[j].vecteur[k]-=pow(nouveaucentres[j].vecteur[k],2);
                    //nouveaucentres[j].vecteur[k]=(centres[j].vecteur[k]-nouveaucentres[j].vecteur[k])/sqrt(v[j].vecteur[k]);
                    }
                }

            }

 // je garde ce centre
//calculer la distance entre les anciens centres et les nouveaux centres pour vérifier qu'ils sont tous inférieurs à un seuil
            int nbre=0;
            for(int i=0; i<nbreCluster; i++)
            {
                if(dist_histo_couleur(centres[i].vecteur,anciencentres[i].vecteur,nbreNiveaux)<PRECISION)
                {
                    nbre++;
                }
            }
            //cout <<" nbre "<<nbre<<"\n";
            if(nbre==nbreCluster) erreur=true;
            //  nouveaucentres.assign(centres.begin(),centres.end());
          /*  for(int i=0; i<3*nbreNiveaux; i++)
            {
                cout<<"centre 0:" << centres[0].vecteur[i]<< " centre 1:" << centres[1].vecteur[i]<< "\n";
            }*/
            /*for(int i=0; i<3*nbreNiveaux; i++)
            {
                cout<<"nouveaucentre " << nouveaucentres[0].vecteur[i] << "\n";
            }*/
              for(int j=0; j<nbreCluster; j++)
            {
                for(int k=0; k<3*nbreNiveaux; k++)
                {
                    anciencentres[j].vecteur[k]= centres[j].vecteur[k];
                    //v[j].vecteur[k]=0;
                }

            }
           // anciencentres.vecteur=centres.vecteur;
            // copy(nouveaucentres.begin(),nouveaucentres.end(),centres.begin());

        }
//Affichage des fichiers de la base ainsi que le cluster auquel ils appartiennent
           for(int i=0; i<nbreCluster; i++)
           {
               for(int j=0; j<imagesClasses.size(); j++)
               {
                   if(imagesClasses[j].classe==i)
                       cout <<"image : " << fn[j] << " cluster numéro " << imagesClasses[j].classe <<"\n";
               }
           }




    }
    waitKey(0);
    return 0;
}





