#I copied this code from AI; trust me it works
import zipfile
import sys

with zipfile.ZipFile(sys.argv[1], 'w') as zipf:
    zipf.write ('rocket.ork', arcname = 'rocket.ork') #'rocket.ork' is the file to zip