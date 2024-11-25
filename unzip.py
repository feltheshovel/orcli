#I copied this code from AI; trust me it works
import zipfile
import sys

# Specify the path to your ZIP file
zip_file_path = sys.argv[1]
# Specify the directory where you want to extract the files
extract_to_path = './'

# Create a ZipFile object
with zipfile.ZipFile(zip_file_path, 'r') as zip_ref:
    # Extract all the contents into the specified directory
    zip_ref.extractall(extract_to_path)