import sqlite3

def init_database(db) :
    conn = sqlite3.connect(db)
    cur = conn.cursor()

    cur.execute("""
    CREATE TABLE IF NOT EXISTS "Utilisateur" (
    "idUser" INTEGER NOT NULL,
    "pseudo" VARCHAR(20) NOT NULL UNIQUE,
    "motDePasse" VARCHAR(20) NOT NULL,
    "salt" VARCHAR(10) NOT NULL,
    PRIMARY KEY ("idUser")
    )""")

    cur.execute("""
    CREATE TABLE IF NOT EXISTS "Partie" (
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
    )""")

    cur.execute("""
    CREATE TABLE IF NOT EXISTS "AchievementDesc" (
        "idsucces" INTEGER NOT NULL,
        "description" VARCHAR(100),
        PRIMARY KEY ("idsucces")
    )""")


    cur.execute("""
    CREATE TABLE IF NOT EXISTS "Achievements" (
        "idpartie" INTEGER NOT NULL,
        "idsucces"  INTEGER NOT NULL,
        FOREIGN KEY ("idpartie") REFERENCES "Partie"("idpartie"),
        FOREIGN KEY ("idsucces") REFERENCES "AchievementDesc"("idsucces"),
        PRIMARY KEY ("idpartie","idsucces")
    )""")
    

    conn.commit()

    cur.execute("SELECT COUNT(idSucces) FROM AchievementDesc")
    data = cur.fetchall()
    if data[0][0] == 0 : # Les données concernant les achievements ne sont pas dans la BD

        cur.execute("""
        INSERT INTO AchievementDesc
            Values (1, 'Gagner 10 parties')
            , (2, 'Gagner 20 parties')
            , (3, 'Gagner 50 parties')
            , (4, 'Gagner 100 parties')
            , (5, 'Gagner 200 parties')
            , (6, 'Gagner 500 parties')
            , (7, 'Jouer 10 parties')
            , (8, 'Jouer 20 parties')
            , (9, 'Jouer 50 parties')
            , (10, 'Jouer 100 parties')
            , (11, 'Jouer 200 parties')
            , (12, 'Jouer 500 parties')
            , (13, 'Gagner une partie en 3 essais ou moins')
            , (14, 'Jouer une partie avec un mot de chaque longueur')
            , (15, 'Gagner une partie avec un mot de chaque longueur')
            , (16, 'Gagner 5 parties consécutives')
            , (17, 'Perdre 3 parties consécutives')
            , (18, 'Gagner une partie lors du dernier essai possible')
        """)

    conn.commit()
    cur.close()
    conn.close()
