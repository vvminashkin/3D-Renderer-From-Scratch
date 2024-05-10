# 3D-Renderer
3D рендерер, написанный с нуля, умеет:
+ Проецирование 3d объектов на экран
+ Интерполяция нормалей вершин по всей поверхности
+ Клиппинг треугольников
+ Построение геодезического многогранника, приближающего сферу
+ Точечный, фоновый источники освещения
+ Система освещения Блинна-Фонга 
+ Добавление пользовательских моделей в формате .obj
+ Перемещение и вращение выбранных объектов: моделей, камеры, источников света
## Клонирование
**Используются unix команды, для других систем использовать их аналоги (для windows можно использовать git bash и Windows PowerShell)**\
В терминале ввести команду:
```shell
git clone --recurse-submodules https://github.com/supernyandev/3D-Renderer-From-Scratch
```
И перейти в папку репозитория\
Для новейших изменений перейти в ветку `dev`:
```shell
git checkout dev
```
И скачать необходимые зависимости:
```shell
git submodule update --init --recursive
```
## Сборка
2 необходимые библиотеки уже скачаны через git submodules, библиотека для оконного приложения SFML скачивается через `cmake FetchContent`
+ На Debian-based OS необходимо установить зависимости SFML:
```shell
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev
```
+ Создать папку для сборки:
```shell
mkdir release
cd release
```
+ Запустить `cmake`:
```shell
cmake .. -DCMAKE_BUILD_TYPE=Release
```
или для `ninja`
```shell
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
```
`cmake` сам скачает SFML
+ Запустить сборку:
```shell
make
```
или
```shell
ninja
```
При возникновении проблем обратиться к [документации SFML](https://www.sfml-dev.org/tutorials/2.6/start-cmake.php) или [общий туториал по работе с SFML](https://www.sfml-dev.org/tutorials/2.6/)

