Part of this program comes from PAM module from google authenticator
These parts are under Apache 2.0 Licence. It may have been modified by myself

Source code can be found here https://code.google.com/p/google-authenticator/source/browse/libpam/

Apache 2.0 licence is available at http://www.apache.org/licenses/LICENSE-2.0

Rest of the code is under WTFPL license. See LICENSE.txt for more information
Source of this is at https://svn.tamdb.net/Navspark-GoogleAuth/
To get access, please email me to mioux-svn@insertcoins.net

This is Ansi C with terminal version, not Arduino/Navspark version.

Run this to compile program (replace "main" with "main.exe" for windows):
gcc -g -Wall -std=c99 *.c -o main
