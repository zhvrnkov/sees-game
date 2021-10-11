# Description
Этот проект содержит два таргета: cli.c и game.c.

### cli.cpp
1. читает юнитов в файла units.csv
2. процессит юнитов (вычисляет сколько юнитов видит юнит)
3. записывает в файл units.out.csf

### game.cpp
1. В этом таргете данные процессинга используются, чтобы отрисовать 2Д поле с юнитами
2. Каждый юнит можно выбрать, кликнув по нему мышкой
3. Выбранный юнит подсвечивается зеленым. У вабранного юнита виден его "видимый сектор"
4. Юниты, которые видит выбранный юнит, подсвечиваются синим
5. Можно зумить на колесо мышки
> Для реализации этого тагрета был расширен алгоритм поиска, и, вместо простого количества, записываются индексы видимых юнитом. Изначально на эту идею подталкнула не игра, а оптимизация алгоритма. Было обнаружено, что количество итерация можно уменьшить (и оптимизировать с O(n^2) до O(n(n-1)?), но т.к. код многопоточный, до использовать единственный коунтер не удавалось, так как race conditions (пробовал использовать атомик, но становилось только медленее). Эта реализация содержится в функции `SeesResult sees` в файле `units.h`.

https://user-images.githubusercontent.com/30472090/136714422-187a8cb6-f7de-43bc-a375-4b7c20200f26.mov

# How to run
Зависимости:
- glm
- opengl, glfw, glew
- OpenMP
``` console
make cli # для запуска cli.cpp

make game # для запуска game.cpp
```
