/*
* Лабораторная работа №9 (2 семестр)
*  
* Выполните рефакторинг кода, который был приведен на лекции. Что понимается под рефакторингом:
*   1.Создание класса BMP.
*   2.Определение методов чтения и записи изображения.
*   3.Разбиение кода на дополнительные функции по вашему усмотрению.
*   4.Переименование переменных по вашему усмотрению.
*   5.Добавить 1 фильтр для изображения.
*/

#include "inc/bmp.hpp"

using namespace sm;

int main()
{
    std::ifstream in("../in.bmp", std::ios::binary);

    BMP image(in);
    BMP copy(image);
    copy.customFilter();

    int newWidth = image.getWidth(),
        newHeight = image.getHeight();
    BMP newImage(newHeight, newWidth);
    newImage.fill(image.getPixels(), newHeight, newWidth);

    std::ofstream copy_bmp("../copy.bmp", std::ios::binary);
    copy.print(copy_bmp);

    std::ofstream new_bmp("../new.bmp", std::ios::binary);
    newImage.print(new_bmp);
}