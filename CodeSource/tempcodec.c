#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//EXEC SQL INCLUDE SQLCA.H;

int codeErreurs(void);

int connexion(void);

int menuPrincipal(void);
void traitementChoix(int);

void recherche(void);
void creerSujet(void);
//void modifSujet(void);

void faire_menu(char*, char*, char*, char*, char*);

//proc code=ANSI_C main.pc
//gcc -o ../Wiki main.c -L$ORACLE_HOME/lib -lclntsh -I$ORACLE_HOME/sdk/include

int main(void)
	{

	int choix_menu_principal;

	if(!connexion())
		{
		printf("Vous avez echoue trop de fois a vous connecter, degage\n");
		return 1;
		}

	printf("\n\nBienvenue sur le Wiki !\n");

	do 	{
		choix_menu_principal = menuPrincipal();
		traitementChoix(choix_menu_principal);
		}while(choix_menu_principal!=4);
	return 0;
	}

/*int codeErreurs(void){
	if(sqlca.sqlcode < 0){
		printf("La commande n'a pas marché.\nErreur : %s\n", sqlca.sqlerrm.sqlerrmc);
		return 1;
	}
	return 0;
}*/

int connexion(void)
	{

//	EXEC SQL BEGIN DECLARE SECTION;
		char login[20];
		char password[20];
//	EXEC SQL END DECLARE SECTION;

	int i=0;

	do {
		printf("Entrez votre login : ");
		fgets(login,sizeof(login),stdin);
		printf("Entrez votre mot de passe : ");
		fgets(password,sizeof(password),stdin);

//		EXEC SQL CONNECT :login IDENTIFIED BY :password;

//		if(!codeErreurs())
			break;

		i++;
	} while(i<3);

	if(i<3)
		{
		printf("Vous êtes maintenant connecté à la base de donnée sous le login : %s\n", login);
		return 1;
		}
	else
		return 0;
	}

int menuPrincipal(void)
	{
	int choix;
	system("clear");
	do
		{
		printf("Menu Principal\n\n");
		printf("Veuillez choisir l'action souhaitee :\n");
		faire_menu("Effectuer une recherche", "Creer un sujet", "Modifier un sujet", "Quitter", " ");

		scanf("%d", &choix);
		system("clear");
		} while(choix<1 || choix>4);
	return choix;
	}

void traitementChoix(int choix)
	{
	if(choix==1)
		recherche();
	else if(choix==2)
		creerSujet();
	//else if(choix==3)
	//	modifSujet();
	
	}
	
void recherche()
	{
	int quitter=0;
	char sujet[255];

	while(!quitter)
		{
		system("clear");
		printf("Quel sujet voulez vous rechercher ?\n");
		printf("--> ");
		while(getchar()!='\n'){}
		fgets(sujet,sizeof(sujet),stdin);
		
		//commande en sql de recherche du sujet
		//recupérer le nb de resultat

		system("clear");
		printf("Que voulez vous faire ?\n");
		faire_menu("Lire un autre article", "Retour au menu principal", " ", " ", " ");

		scanf("%d", &quitter);
		quitter--;
		system("clear");
		}
	}
	
void creerSujet()
	{
	char sujet[255];
	char sujetliens[5][255];
	int nbsujetlies=0;

	char tempTexte[5000];
	char texte[500000];

	char theme[255];

	int proteger=0;

	int curseur=0;
	int curseurFi=0;

	int sortir=0;
	int choix=0;
	
	system("clear");
	printf("Entrez le sujet sur lequel porteras votre article\n");//On récupère le sujet
	printf("--> ");
	while(getchar()!='\n'){}
	fgets(sujet,sizeof(sujet),stdin);

	for(curseur=0; curseur<500000; curseur++)
		texte[curseur]='\0';
	
	printf("\n\nEntrez le texte à ajouter dans l'article (combinaison .. pour arrêter la saisie) :\n");

	for(int nblignes=0;nblignes<100 && !sortir;nblignes++)
		{
		for(curseur=0;curseur<5000; curseur++)//on récupère la ligne
			tempTexte[curseur]='\0';
		fgets(tempTexte,sizeof(tempTexte),stdin);
		for(curseur=0;curseurFi<500000 && tempTexte[curseur]!='\0' && curseur<5000 ; curseurFi++)//On verifie sue .. n'as pas ete saisi a la fin
			{
			if(tempTexte[curseur]=='.' && tempTexte[curseur-1]=='.')
				{
				sortir=1;//si oui on sort
				break;
				}
			texte[curseurFi]=tempTexte[curseur];//on associe la ligne a la suite du texte final
			curseur++;
			}
		texte[curseurFi-1]=' ';//on ajoute un espace pour retirer le retour à la ligne
		texte[curseurFi]='\n';//on ajoute un retour à la ligne a la fin de la ligne pour retirer le \0
		curseurFi++;
		}
	texte[curseurFi-2]='.';//ajouter un point à la place de l'espace sur la dernière ligne.

	do{
		system("clear");
		printf("Voici le resultat de votre saisie :\n");//on attend validation
		printf("\nSujet : %s\n\n%s\n", sujet, texte);
		printf("Que faire :\n");
		faire_menu("Continuer", "Annuler", " ", " ", " ");
		scanf("%d",&choix);
		}while(choix!=1 && choix!=2);
	if(choix!=2)
		{
		do{	
			system("clear");
			printf("Voulez vous protéger le contenu de ce topic ?\n");//on demande si le contenu doit etre protégé a la modif
			faire_menu("Non", "Oui", " ", " ", " ");
			scanf("%d", &proteger);
			}while(proteger!=1 && proteger!=2);
		proteger--;

		system("clear");
		printf("Entrez le thème de votre ebauche : ");
		while(getchar()!='\n'){}
		fgets(theme,sizeof(theme),stdin);


		do{
			system("clear");
			printf("Voulez vous lier ce sujet à d'autres sujets ?\n");
			faire_menu("Non", "Oui", " ", " ", " ");
			scanf("%d", &choix);
				if(choix==2)
				{
				system("clear");
				printf("Vous avez deja lié %d sujets.\nVous pouvez lier jusqu'à 5 sujets.\n\nEntrez le sujet que vous souhaitez lier : ",nbsujetlies);
				while(getchar()!='\n'){}
				fgets(sujetliens[nbsujetlies],sizeof(sujetliens[nbsujetlies]),stdin);
				//ajouter les commandes sql : verifier que sujet exite
				//si exite alors demander de valider le choix
				//sinon demande de recommancer
				nbsujetlies++;
				}
			}while(choix!=1 && nbsujetlies<5);
		}

	//ajouter les commandes sql : insérer dans sujet le titre, la valeur de protéger, et le pseudo du créateur
	//ajouter les commandes sql : insérer dans contenu le titre, la date, l'auteur et le texte
	//ajouter les commandes sql : insérer dans liens le titre du sujet et le titre du sujet lié

	//pas sur du fonctionnement de la table themes donc a voir :
	//ajouter les commandes sql : insérer dans thème le thème père et le thème courant OU le thème courant et son thème fils si c'est lui le papa

	//ajouter les commandes sql : insérer dans classification le sujet et le thème

	}
 
void faire_menu(char* c1, char* c2, char* c3, char* c4, char* c5)
	{
	if(c1[0]!=' ')
		printf("\t1 - %s\n", c1);
	if(c2[0]!=' ')
		printf("\t2 - %s\n", c2);
	if(c3[0]!=' ')
		printf("\t3 - %s\n", c3);
	if(c4[0]!=' ')
		printf("\t4 - %s\n", c4);
	if(c5[0]!=' ')
		printf("\t5 - %s\n", c5);
		
	printf("--> ");
	}










