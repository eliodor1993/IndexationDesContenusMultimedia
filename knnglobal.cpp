#include <cv.h>
#include <map>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
#define DIMENSION 8
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
//chercher les tonalité max et min d'une image en niveau de gris, une seule channel

#define RACINE "test/"
//distance euclidienne d'une individu à un autre

//moments bruts
double moment(int p, int q, Mat image)
{
    double result=0.0;
    for(int x = 0; x < image.rows; x++)
    {
        for(int y = 0; y < image.cols; y++)
        {

            result+= pow (x, p)*pow(y,q)*((int)image.at<uchar>(x,y));

        }
    }

    return result;
}
//moments centrés
double centralmoment(int p, int q, Mat image)
{
    double result=0.0;
    double xmean= moment(1,0,image)/moment(0,0,image);
    double ymean= moment(0,1,image)/moment(0,0,image);
    for(int x = 0; x < image.rows; x++)
    {
        for(int y = 0; y < image.cols; y++)
        {

            result+= pow ((x-xmean), p)*pow((y-ymean),q)*((int)image.at<uchar>(x,y));
        }
    }

    return result;
}
//invariant à l'échelle
double scalemoment(int p, int q, Mat image)
{
    double result=centralmoment(p,q,image)/pow(centralmoment(0,0,image),1+(p+q)/2);


    return result;
}
//Invariant à la rotation
double* rotationmoment(Mat image)
{
    double* vecteur=new double[8];
    vecteur[0]=scalemoment(2,0,image)+scalemoment(0,2,image);
    vecteur[1]=pow(scalemoment(2,0,image)-scalemoment(0,2,image),2)+4*pow(scalemoment(1,1,image),2);
    vecteur[2]=pow(scalemoment(3,0,image)-3*scalemoment(1,2,image),2)+pow(3*scalemoment(2,1,image)-scalemoment(0,3,image),2);
    vecteur[3]=pow(scalemoment(3,0,image)+scalemoment(1,2,image),2)+pow(scalemoment(2,1,image)+scalemoment(0,3,image),2);
    vecteur[4]=(scalemoment(3,0,image)-3*scalemoment(1,2,image))*(scalemoment(3,0,image)+scalemoment(1,2,image))*(pow(scalemoment(3,0,image)+scalemoment(1,2,image),2)-3*pow(scalemoment(2,1,image)+scalemoment(0,3,image),2))+
               (3*scalemoment(2,1,image)-scalemoment(0,3,image))*(scalemoment(2,1,image)+scalemoment(0,3,image))*(3*pow(scalemoment(3,0,image)+scalemoment(1,2,image),2)-pow(scalemoment(2,1,image)+scalemoment(0,3,image),2));
    vecteur[5]=(scalemoment(2,0,image)-scalemoment(0,2,image))*(pow(scalemoment(3,0,image)+scalemoment(1,2,image),2)-pow(scalemoment(2,1,image)+scalemoment(3,0,image),2))+4*scalemoment(1,1,image)*(scalemoment(3,0,image)+scalemoment(1,2,image))*(scalemoment(2,1,image)+scalemoment(0,3,image));
    vecteur[6]=(3*scalemoment(2,1,image)-scalemoment(0,3,image))*(scalemoment(3,0,image)+scalemoment(1,2,image))*(pow(scalemoment(3,0,image)+scalemoment(1,2,image),2)-3*pow(scalemoment(2,1,image)+scalemoment(0,3,image),2))-
               (scalemoment(3,0,image)-3*scalemoment(1,2,image))*(scalemoment(2,1,image)+scalemoment(0,3,image))*(3*pow(scalemoment(3,0,image)+scalemoment(1,2,image),2)-pow(scalemoment(2,1,image)+scalemoment(0,3,image),2));
    vecteur[7]=scalemoment(1,1,image)*(pow(scalemoment(3,0,image)+scalemoment(1,2,image),2)-pow(scalemoment(0,3,image)+scalemoment(2,1,image),2)) -(scalemoment(2,0,image)-scalemoment(0,2,image))*(scalemoment(3,0,image)+scalemoment(1,2,image))*(scalemoment(0,3,image)-scalemoment(2,1,image));
    return vecteur;
}

double dist_hu(double* individu1, double* individu2, int taille)
{

    double distance = 0;
    for (int i = 0; i < taille; i++)
    {
        distance += (individu1[i] - individu2[i])*(individu1[i] - individu2[i]);
    }
    distance = sqrt(distance)/taille;
    return distance;
}
float dist_histo_couleur(int* individu1, int* individu2, int taille)
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

int* histogrammeCouleurEchantillon(Mat image, char * imageName, int nbreNiveaux)
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

// maintenant je parcours toute la ligne restante pour en extraire l'information sur le niveau de gris
    /* for(int k=0; k<nbreNiveaux; k++)
     {

         p0b=Point(k*segment,imageHistoBleue.rows);
         p1b=Point(k*segment,imageHistoBleue.rows-histogramb[k]*imageHistoBleue.rows/(maxb));
         p0g=Point(k*segment,imageHistoVert.rows);
         p1g=Point(k*segment,imageHistoVert.rows-histogramg[k]*imageHistoVert.rows/(maxg));
         p0r=Point(k*segment,imageHistoRouge.rows);
         p1r=Point(k*segment,imageHistoRouge.rows-histogramr[k]*imageHistoRouge.rows/(maxr));
         ligne(imageHistoBleue,p0b,p1b,100,0,0,1);
         ligne(imageHistoVert,p0g,p1g,0,100,0,1);
         ligne(imageHistoRouge,p0r,p1r,0,0,100,1);
     }

     namedWindow(imageName,CV_WINDOW_AUTOSIZE);
     imshow(imageName,image);
     //je fabrique les matrices et les fenêtres qui les contiendrons
     char imageNameHistoBleue[80]="histobleueechantillon_";
     strcat(imageNameHistoBleue,imageName);
     namedWindow(imageNameHistoBleue,CV_WINDOW_AUTOSIZE);
     char imageNameHistoVert[80]="histovertechantillong_";
     strcat(imageNameHistoVert,imageName);
     namedWindow(imageNameHistoVert,CV_WINDOW_AUTOSIZE);
     char  imageNameHistoRouge[160]="historougeechantillon_";
     strcat(imageNameHistoRouge,imageName);
     namedWindow(imageNameHistoRouge,CV_WINDOW_AUTOSIZE);

     //afficher les profils dans les fenêtres
     imshow(imageNameHistoRouge,imageHistoRouge);
     imshow(imageNameHistoVert,imageHistoVert);
     imshow(imageNameHistoBleue,imageHistoBleue);
     // cout <<"maxg: " << maxg <<" maxr: " << maxr << " maxb: " << maxb;*/

    int* vecteur=new int[3*nbreNiveaux];
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

    Mat image,image1;
    //ceci représente le nom de l'image fournie en entrée du programme
    char* imageName = argv[1];
    // char format[5] = ".png";
    char filename[150] = RACINE;

    strcat(filename, imageName);
    cout << filename << "\n";
    int nbreNiveaux;
    float poids=0.5;
    image= imread(filename);
    image1= imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    nbreNiveaux=atoi(argv[2]);
    int ppVoisins=atoi(argv[3]);
    if(argc==5)
        poids = atoi(argv[4]);
    if( !image.data || !image1.data)
    {


        cout << aide << "\n";
        return -1;

    }



    //image de travail utilisée pour calculer le profil d'intensité je fais une copie avant de faire le tracé sur l'autre image afin de ne 	pas troncer les résultats



    if((image.channels()==1))
    {
        // pour les images en niveau de gris


    }
    else
    {

        //pour les images avec 3 channels
        Vec3b pixel;
        int r,g,b;
// Je teste le type d'image la couleur
        int gris=grise(image);
        if(gris!=0)
        {

            //  histogrammeCouleur(image,imageName);
            double* vecteurhu= rotationmoment(image1) ;

            int* vecteur= histogrammeCouleurEchantillon(image,imageName,nbreNiveaux) ;
            /*  for(int i=0; i<3*nbreNiveaux; i++)
              {
                  cout << vecteur[i] << "\n";
              }*/

            //histogrammeCouleurEchantillon(image1,imageName1,nbreNiveaux);
            vector<cv::String> fn;
            glob("test/*.png", fn, false);

            vector<Mat> images;
            vector<Mat> images1;
            vector<float> distances;
            size_t count = fn.size(); //number of png files in images folder
            int j=0;
            for (size_t i=0; i<count; i++)
            {

                if (strcmp(filename,fn[i].c_str())!=0)
                {
                    images.push_back(imread(fn[i]));
                    images1.push_back(imread(fn[i], CV_LOAD_IMAGE_GRAYSCALE));
                    //   cout  <<  " fn " << fn[i] << "\n";
                    double* vecteurBaseHu= rotationmoment(images1[j]) ;
                    int* vecteurBase= histogrammeCouleurEchantillon(images[j],const_cast<char*>(fn[i].c_str()),nbreNiveaux) ;
                    /*for(int j=0; j<3*nbreNiveaux; j++)
                     {
                       cout << vecteurBase[j] << "\n";
                     }*/
                    j++;
                    distances.push_back(poids*dist_histo_couleur(vecteur,vecteurBase,nbreNiveaux)+(1-poids)*dist_hu(vecteurhu,vecteurBaseHu,DIMENSION));
                }
            }
            //contient la classe des ppVoisins plus proches
            cv::String* classeKNN=new cv::String[ppVoisins];



            /*    float* distanceTriee=new float[3*nbreNiveaux];
            for(int j=0; j<3*nbreNiveaux; j++)
                                 {
                                  distanceTriee[j]=distances[j];
                                 }*/
            size_t count1= distances.size();
//classer les elements par ordre croissant
//pos_min is short for position of min
            float pos_min,temp;
            cv::String tempfn,classeEchantillon;
            for (int i=0; i < count1-1; i++)
            {
                pos_min = i;//set pos_min to the current index of array

                for (int j=i+1; j < count1; j++)
                {

                    if (distances[j] < distances[pos_min])
                        pos_min=j;
                    //pos_min will keep track of the index that min is in, this is needed when a swap happens
                }

                //if pos_min no longer equals i than a smaller value must have been found, so a swap must occur
                if (pos_min != i)
                {
                    temp = distances[i];
                    tempfn = fn[i];
                    distances[i] = distances[pos_min];
                    distances[pos_min] = temp;
                    fn[i] = fn[pos_min];
                    fn[pos_min] = tempfn;
                }
            }
            std::map<string,int> classes;

            int xx=0;
            if (strcmp(filename,fn[xx].c_str())==0)
                xx++;
            //affichage des ppVoisin plus proches
            for (size_t j=0; j<ppVoisins; j++)
            {
                cv::String classeIndividu;
                char* s;
                string fichier=fn[xx];
                s = strtok (const_cast<char*>(fichier.c_str()),"/");
                s = strtok (NULL, "/");

                s = strtok (s,"__");
                classeIndividu=s;
                cout << ppVoisins <<  " plus proches voisins  " <<  "Fichier " << fn[xx] <<  " Distance " << distances[j] <<  " classe " << classeIndividu << "\n";

                if(classes[classeIndividu]!=NULL)
                {
                    classes[classeIndividu]=classes[classeIndividu]+1;

                }
                else
                {
                    classes[classeIndividu]=1;
                }
                //recherche des différentes classes et mise dans le map
                xx++;
            }
// cout <<  " nbre d'élément de la classe obj1 " << filename <<  " classe: " << classes["obj1"] << "\n";


            // chercher la classe dominante
            int max=-1;
            string classeIndividu;
            for (std::map<string,int>::iterator it=classes.begin(); it!=classes.end(); ++it)
            {
                //std::cout << it->first << " => " << it->second << '\n';
                if(it->second > max)
                {
                    classeIndividu=it->first;
                }
            }
            cout <<  " classification de l'image en entrée " << filename <<  " classe " << classeIndividu << "\n";
        }
        else
        {

        }



    }







    waitKey(0);
    return 0;
}
