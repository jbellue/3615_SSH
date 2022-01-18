#ifndef L10N_H
#define L10N_H

#include <WString.h>

enum Languages {
    ENGLISH,
    FRENCH,
    SPANISH,

    LANGUAGES_LAST
};
enum L10N_STRINGS {
    SSH_PAGE_TITLE,
    SSH_PAGE_HOST,
    SSH_PAGE_USERNAME,
    SSH_PAGE_PASSWORD,
    SSH_PAGE_WIFI_OPTIONS,
    SSH_PAGE_LANGUAGE,

    WIFI_PAGE_TITLE,
    WIFI_PAGE_CONNECTED_TO,
    WIFI_PAGE_DISCONNECT,
    WIFI_PAGE_SCANNING,
    WIFI_PAGE_NO_NETWORK_FOUND,
    WIFI_PAGE_ONE_NETWORK_FOUND,
    WIFI_PAGE_NETWORKS_FOUND,
    WIFI_PAGE_CHOICE_THEN,
    WIFI_PAGE_DISCONNECTING,
    WIFI_PAGE_PASSWORD,
    WIFI_PAGE_CONNECTING,
    WIFI_PAGE_CONNECTED,
    WIFI_PAGE_CONNECTION_FAILED,
    WIFI_PAGE_CHECK_PASSWORD,
    WIFI_PAGE_TO_SCAN_AGAIN,
    WIFI_PAGE_GO_BACK,

    LANGUAGE_PAGE_TITLE,

    L10N_LAST_STRING
};

class L10n {
public:

    L10n():
        selectedLanguageIndex(Languages::ENGLISH){};
    L10n(Languages languageIndex):
        selectedLanguageIndex(languageIndex){};

    void setLanguage(Languages l) { selectedLanguageIndex = l; };

    String get(const uint16_t index) {
        return languageStore[index][selectedLanguageIndex];
    }
private:
    Languages selectedLanguageIndex;

    String languageStore[L10N_LAST_STRING][LANGUAGES_LAST] = {
        { "3615 SSH client",    "3615 SSH client",      "3615 SSH client"       },
        { "Host:",              "Hôte :",               "Servidor:"             },
        { "Username:",          "Nom d'utilisateur :",  "Nombre de usuario:"    },
        { "Password:",          "Mot de passe :",       "Contrasena:"           },
        { ": WiFi options",     " : Options WiFi",      ": Opciones WiFi"       },
        { ": Language",         " : Langue",            ": Idioma"              },
        { "WiFi Options",       "Options WiFi",         "Opciones WiFi"         },
        { "Connected to ",      "Connecté à ",          "Conectado a "          },
        { ": disconnect",       " : se déconnecter",    ": desconectar"          },
        { "Scanning...",        "Scan en cours...",     "Escaneando las redes..."},
        { "No network found",   "Aucun réseau trouvé",  "No encontró red"       },
        { "1 network found:",   "1 réseau trouvé :",    "1 red encontrada"      },
        { "networks found:",    "réseaux trouvés :",    "redes encontradas:"    },
        { "Choice then ",       "Choix puis ",          "Elegir entonces: "     },
        { "Disconnecting...",   "Déconnexion...",       "Desconnectando..."     },
        { "Password:",          "Mot de passe :",       "Contrasena"            },
        { "Connection...",      "Connexion...",         "Connectando..."        },
        { "Connected!",         "Connecté !",           "Connectado!"           },
        { "Connection failed.", "Impossible de se connecter.", "Conexión fallida!"},
        { "Check password.",    "Vérifier le mot de passe.", "Comprobar la contrasena."},
        { " to scan again",     " pour scanner à nouveau", " para volver a escanear"},
        { ": back",             " : retour",            ": volver"},
        { "Language",           "Langue",               "Idioma"                }
    };
};

#endif