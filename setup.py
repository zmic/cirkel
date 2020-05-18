# -*- coding: utf-8 -*-

from setuptools import setup
    
from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
class bdist_wheel(_bdist_wheel):
    def finalize_options(self):
        _bdist_wheel.finalize_options(self)
        self.root_is_pure = False
            
with open('README.md', encoding='utf-8') as f:
    README_TEXT = f.read()
    
setup(
    name='cirkel',
    version='0.2',
    packages=['cirkel'],
    python_requires='>=3.5',
    description='Automata plotting circles on canvas',
    long_description = README_TEXT,    
    long_description_content_type='text/markdown',
    url='https://github.com/zmic/cirkel',
    author='Michael Vanslembrouck',
    license='MIT',
    package_data = {
        '': ['example/*.py', 'ccirkel.pyd'],
    },    
    cmdclass={'bdist_wheel': bdist_wheel},    
    classifiers=[
        'Development Status :: 3 - Alpha',
    ],
    install_requires=['numpy', 'imageio']
)

      