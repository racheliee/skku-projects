import os
import zipfile
import shutil

def zip_all_in_folder(folder_path, base_path, output_zip):
    # Create a ZIP file
    with zipfile.ZipFile(output_zip, 'w', zipfile.ZIP_DEFLATED) as zipf:
        # Walk through the directory
        for root, dirs, files in os.walk(folder_path):
            for file in files:
                # Create the full path to the file
                file_path = os.path.join(root, file)
                # Write the file to the zip, adjust the arcname to customize how the paths are stored
                zipf.write(file_path, arcname=os.path.relpath(file_path, start=base_path))

if os.path.exists('submission.zip'):
    os.remove('submission.zip')
with open('README.md', 'r') as f:
    assert f.readline().startswith("# Assignment: Creating a Tax Form"), "You should run zipit from the assignment folder"
base_path = os.getcwd()
folder_path = os.path.join(base_path, 'assignment')
zip_all_in_folder(folder_path, base_path, 'submission.zip')
