import os
import zipfile
import shutil

SKIPPED = ['/databases/', 'demo.mp4']

def zip_all_in_folder(folder_path, base_path, output_zip):
    # Create a ZIP file
    with zipfile.ZipFile(output_zip, 'w', zipfile.ZIP_DEFLATED) as zipf:
        # Walk through the directory
        for root, dirs, files in os.walk(folder_path):
            for file in files:
                # Create the full path to the file
                file_path = os.path.join(root, file)
                if any([s in file_path for s in SKIPPED]):
                    continue
                # Write the file to the zip, adjust the arcname to customize how the paths are stored
                zipf.write(file_path, arcname=os.path.relpath(file_path, start=base_path))

if os.path.exists('submission.zip'):
    os.remove('submission.zip')
base_path = os.getcwd()
folder_path = os.path.join(base_path, 'apps')
# If apps is not in this folder, quit. 
assert os.path.exists(folder_path), 'Folder "apps" not found in the current directory.'
zip_all_in_folder(folder_path, base_path, 'submission.zip')
