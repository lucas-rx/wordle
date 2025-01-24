CREATE TABLE "Utilisateur" (
    "idUser" INTEGER NOT NULL,
    "pseudo" VARCHAR(20) NOT NULL UNIQUE,
    "motDePasse" VARCHAR(20) NOT NULL,
    "salt" VARCHAR(10) NOT NULL,
    PRIMARY KEY ("idUser")
);

CREATE TABLE "Partie" (
    "idPartie" INTEGER NOT NULL,
    "idUser" INTEGER NOT NULL,
    "motSecret" VARCHAR(15) NOT NULL,
    "pts_experience"  INTEGER,
    "nb_essais_max" INTEGER,
    "nb_coups"      INTEGER,
    "longueur_mot"   INTEGER,
    "modejeu"        VARCHAR(10) NOT NULL,
    PRIMARY KEY ("idPartie"),
    FOREIGN KEY ("idUser") REFERENCES "Utilisateur"("idUser")
);

CREATE TABLE "AchievementDesc" (
    "idsucces" INTEGER NOT NULL,
    "description" VARCHAR(100),
    PRIMARY KEY ("idsucces")
);

CREATE TABLE  "Achievements" (
    "idpartie" INTEGER NOT NULL,
    "idsucces"  INTEGER NOT NULL,
    FOREIGN KEY ("idpartie") REFERENCES "Partie"("idpartie"),
    FOREIGN KEY ("idsucces") REFERENCES "AchievementDesc"("idsucces"),
    PRIMARY KEY ("idpartie","idsucces")
);

