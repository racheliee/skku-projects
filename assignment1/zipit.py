import os
import zipfile
import shutil

def zip_all_in_folder(base_path, output_zip):
    # Create a ZIP file
    with zipfile.ZipFile(output_zip, 'w', zipfile.ZIP_DEFLATED) as zipf:
        fn = 'index.html'
        file_path = os.path.join(base_path, fn)
        zipf.write(file_path, arcname='index.html')
        
if os.path.exists('submission.zip'):
    os.remove('submission.zip')
base_path = os.getcwd()
zip_all_in_folder(base_path, 'submission.zip')
