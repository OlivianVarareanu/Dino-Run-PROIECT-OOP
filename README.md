# INSTRUCTIONS 

[ENGLISH]
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
a) Download the project from the GitHub repository:
b) To access the application, you will need to open the "Proiect.sln" file with Visual Studio Community Edition 2022 or a later version.
c) After opening it will be necessary to set from Visual Studio the specific locations of the FSML library, which is inside the same folder.
d) In the Solution Explorer tab in Visual Studio, select the Project file, then right-click -> Properties
e) In the Project Property Pages window, there will be several Properties on the left side. In Configuration Properties -> General we will make sure that Windows SDK Version is version 10.0 and Platform Toolset is Vistual Studio 2022 (v143).
f) In Configuration Properties -> Debugging -> Environment we will write "PATH=", followed by the path corresponding to the bin folder. Example: PATH=D:\Proiect\SFML-2.5.1\bin , after which we press the enter key and the Apply button.
g) In C/C++ -> General at Additional Inlcude Directories we will set the path corresponding to the include folder from the SFML-2.5.1 library. Example: D:\Project\SFML-2.5.1\include, after which we press the enter key and the Apply button.
h) In Linker -> General at Additional Library Directories we will set the path corresponding to the lib folder from the SFML-2.5.1 library. Example: D:\Proiect\SFML=2.5.1\lib, after which we press the enter key and the Apply button.
i) In Linker -> Input at Additional Dependencies we will set '' sfml-graphics-d.lib;sfml-window-d.lib;sfml-audio-d.lib;sfml-network-d.lib;sfml-system- d.lib;%(AdditionalDependencies) '' , after which we press the enter key and the Apply button.
j) After we have set the dependencies to this library we can close the properties window and run the application through the Local Windows Debugger button.
k) Enjoy!
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
[ROMANIAN]
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
a)	Descărcați proiectul de pe repository-ul de pe GitHub: 
b)	Pentru accesarea aplicației va fi nevoie de deschiderea fișierului “Proiect.sln” cu Visual Studio Community Edition 2022 sau versiune ulterioară.
c)	După deschidere va fi necesară setarea din Visual Studio a locațiilor specifice ale librăriei FSML, care se află în interiorul aceluiași folder. 
d)	În tab-ul Solution Explorer din Visual Studio selectăm fișierul Proiect, după care apăsăm Click-dreapta -> Properties
e)	În fereastra Proiect Property Pages, în partea stanga se vor afla mai multe Proprietăți. În Configuration Properties -> General ne vom asigura că Windows SDK Version este versiunea 10.0 și Platform Toolset este Vistual Studio 2022 (v143).
f)	În Configuration Properties -> Debugging -> Environment vom scrie “ PATH= “ , urmat de path-ul corespunzător folderului bin. Exemplu: PATH=D:\Proiect\SFML-2.5.1\bin , după care apăsăm tasta enter și butonul Apply.
g)	În C/C++ -> General la Additional Inlcude Directories vom seta path-ul corespunzător folderului include din librăria SFML-2.5.1. Exemplu: D:\Proiect\SFML-2.5.1\include, după care apăsăm tasta enter și butonul Apply.
h)	În Linker -> General la Additional Library Directories vom seta path-ul corespunzătpr folderului lib din librăria SFML-2.5.1. Exemplu: D:\Proiect\SFML=2.5.1\lib, după care apăsăm tasta enter și butonul Apply.
i)	În Linker -> Input la Additional Dependencies vom seta ‘’ sfml-graphics-d.lib;sfml-window-d.lib;sfml-audio-d.lib;sfml-network-d.lib;sfml-system-d.lib;%(AdditionalDependencies) ‘’ , după care apăsăm tasta enter și butonul Apply.
j)	După ce am setat dependențele față de această librărie putem închide fereastra de proprietăți și rula aplicația prin butonul Local Windows Debugger.
k)	Enjoy!
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
