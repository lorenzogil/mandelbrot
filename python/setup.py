from setuptools import find_packages, setup


setup(name='mandelbrot',
      version='0.1',
      description='Mandelbrot set fractal',
      url='https://github.com/lorenzogil/mandelbrot',
      author='Lorenzo Gil Sanchez',
      license='GPLv3',
      packages=find_packages(),
      install_requires=['PySDL2==0.9.4'],
      entry_points={
          'console_scripts': [
              'mandelbrot=mandelbrot:main',
          ]
      })
