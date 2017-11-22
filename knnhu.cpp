#include <cv.h>
#include <map>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
/**
 ELIODOR Ednalson Guy Mirlin
 Promo 21 IFI
 2016-2017
Ce petit programme permet de calculer le moment de hu des images puis de calculer la distance entre l'image  passée 
en paramètre et l'ensemble des images de la base et enfin prédire la classe de l'image d'entrée. exple: obj1, obj2 

              +++++++++++++++++++++++++++++++++++++++++
              -----------------------------------------
   Ce code est 100% écrit par nous meme et ne peut etre utilisé qu'à des fins académiques
              -----------------------------------------
              +++++++++++++++++++++++++++++++++++++++
**/


#define RACINE "test/"
#define DIMENSION 8
//distance euclidienne d'une individu à un autre

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

int main(int argc, char** argv)
{
    string aide="Erreur";

    Mat image,imageGrey;
    //ceci représente le nom de l'image fournie en entrée du programme
    char* imageName = argv[1];
    // char format[5] = ".png";
    char filename[150] = RACINE;

    strcat(filename, imageName);
    cout << filename << "\n";
    //int nbreNiveaux=8;

    image= imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

    
    int ppVoisins=atoi(argv[2]);
      //  nbreNiveaux=atoi(argv[2]);
    if( !image.data)
    {

        image.release();
        cout << aide << "\n";
        return -1;

    }
    //conversion en niveau de gris

    if (argc==3)
    {



        //image de travail utilisée pour calculer le profil d'intensité je fais une copie avant de faire le tracé sur l'autre image afin de ne 	pas troncer les résultats

        // double* vecteur;
        //cout <<"moment " <<  rotationmoment(image) << "\n";
        double* vecteur= rotationmoment(image) ;
       /* for(int i=0; i<DIMENSION; i++)
        {
            cout << vecteur[i] << "\n";
        */

        //rotationInvariant(image1,imageName1,DIMENSION);
        vector<cv::String> fn;
        glob("test/*.png", fn, false);

        vector<Mat> images;
        vector<double> distances;
        int j=0;
        size_t count = fn.size(); //number of png files in images folder
        // cout  <<  " test " <<count << "\n";
        for (size_t i=0; i<count; i++)
        {
            if (strcmp(filename,fn[i].c_str())!=0)
            {
               
                images.push_back(imread(fn[i], CV_LOAD_IMAGE_GRAYSCALE));
                 // scout  <<  " fn " << fn[i] << "\n";
                

                double* vecteurBase= rotationmoment(images[j]) ;
                /*for(int j=0; j<DIMENSION; j++)
                 {
                   cout << vecteurBase[j] << "\n";
                 }*/

                distances.push_back(dist_hu(vecteur,vecteurBase,DIMENSION));
                j++;
            }
        }
        //contient la classe des ppVoisins plus proches
        cv::String* classeKNN=new cv::String[ppVoisins];
        size_t count1= distances.size();
        //classer les elements par ordre croissant
        //pos_min is short for position of min
        double pos_min,temp;
        string classeIndividu;
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
        //affichage des ppVoisin plus proches
        for (size_t j=0; j<ppVoisins; j++)
        {
  if (strcmp(filename,fn[xx].c_str())==0)
            xx++;

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

      xx++;
            //recherche des différentes classes et mise dans le map

        }
        // cout <<  " nbre d'élément de la classe obj1 " << filename <<  " classe: " << classes["obj1"] << "\n";


        // chercher la classe dominante
        int max=-1;

        for (std::map<string,int>::iterator it=classes.begin(); it!=classes.end(); ++it)
        {
            //std::cout << it->first << " => " << it->second << '\n';
            if(it->second > max)
            {
                classeIndividu=it->first;
            }
        }


        cout <<  "- classification de l'image en entrée -" << filename <<  " -classe- " << classeIndividu << "\n";
    }
    else
    {

    }

    waitKey(0);
    return 0;
}

