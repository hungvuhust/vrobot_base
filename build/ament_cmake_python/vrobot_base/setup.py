from setuptools import find_packages
from setuptools import setup

setup(
    name='vrobot_base',
    version='0.0.0',
    packages=find_packages(
        include=('vrobot_base', 'vrobot_base.*')),
)
