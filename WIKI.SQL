create table sujet(
titre varchar(255) primary key,
protege number(1) check(protege in (0,1)),
createur varchar(8) default user);

create table contenu(
titre varchar(255) references sujet(titre),
datemodif timestamp default sysdate,
auteur varchar(8)  default user,
texte long not null,
primary key (titre,datemodif));

create table demande(
titre varchar(255) references sujet(titre),
datemodif timestamp default sysdate,
demandeur varchar(8)  default user,
texte long not null,
primary key (titre,datemodif,demandeur));

create table lecture(
titre varchar(255) references sujet(titre),
datelecture timestamp default sysdate,
lecteur varchar(8)  default user,
primary key (titre,lecteur,datelecture));

create table liens(
titre1 varchar(255) references sujet(titre),
titre2 varchar(255) references sujet(titre),
primary key (titre1,titre2));

create table theme(
themefils varchar(255) primary key references sujet(titre),
themepere varchar(255) references sujet(titre));

create table classification(
titre varchar(255) references sujet(titre),
theme varchar(255) references theme(themefils),
primary key (titre,theme));


create view nouveausujet as (select titre, protege from sujet);
grant insert on nouveausujet to public;
grant select on sujet to public;

grant select on contenu to public;

create view modificationcontenu as 
(select titre,texte from contenu 
where titre in (select titre from sujet where protege=0 or createur=user))
with check option;
grant insert on modificationcontenu to public;

create view ecriredemande as 
(select titre,texte from demande 
where titre in (select titre from sujet where protege=1 and createur!=user))
with check option;

grant insert on ecriredemande to public;

create view liredemande as 
(select * from demande 
where titre in (select titre from sujet where protege=1 and createur=user));
grant select,delete on liredemande to public;  

grant select on lecture to public;
create view nouvellelecture as (select titre from lecture);
grant insert on nouvellelecture to public;


grant insert,select on liens to public;
grant insert,select on theme to public;
grant insert,select on classification to public;
