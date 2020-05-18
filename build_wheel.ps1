
if (0)
{
    $pips = @(
        "c:\python33\Scripts\pip.exe",
        "c:\python33-32\Scripts\pip.exe",
        "c:\python34\Scripts\pip.exe",
        "c:\python34-32\Scripts\pip.exe",
        "c:\python35\Scripts\pip.exe",
        "c:\python35-32\Scripts\pip.exe",
        "c:\python36\Scripts\pip.exe",
        "c:\python36-32\Scripts\pip.exe",
        "c:\python37\Scripts\pip.exe",
        "c:\python37-32\Scripts\pip.exe",
        "c:\python38\Scripts\pip.exe",
        "c:\python38-32\Scripts\pip.exe"
    )

    $pips.foreach(
        {
            & $_ install numpy
        }
    )
}



if (1)
{
    Remove-Item .\dist -Force -Recurse -erroraction ignore
    Remove-Item .\build -Force -Recurse -erroraction ignore

    copy cirkel2\ccirkel_cp35_x86.pyd cirkel\ccirkel.pyd
    c:\python35-32\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse

    copy cirkel\ccirkel_cp35_x64.pyd cirkel\ccirkel.pyd
    c:\python35\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse

    copy cirkel\ccirkel_cp36_x86.pyd cirkel\ccirkel.pyd
    c:\python36-32\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse

    copy cirkel\ccirkel_cp36_x64.pyd cirkel\ccirkel.pyd
    c:\python36\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse

    copy cirkel\ccirkel_cp37_x86.pyd cirkel\ccirkel.pyd
    c:\python37-32\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse

    copy cirkel\ccirkel_cp37_x64.pyd cirkel\ccirkel.pyd
    c:\python37\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse

    copy cirkel\ccirkel_cp38_x86.pyd cirkel\ccirkel.pyd
    c:\python38-32\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse

    copy cirkel\ccirkel_cp38_x64.pyd cirkel\ccirkel.pyd
    c:\python38\python.exe setup.py bdist_wheel
    Remove-Item .\build -Force -Recurse
}

### C:\python37\Scripts\twine.exe upload --repository-url https://upload.pypi.org/legacy/ dist/*

