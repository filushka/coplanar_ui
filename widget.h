/*
This program allow to find out if given points are coplanar

Copyright (C) <2016>  <Elena Filenko>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Point
{
public:
    double x;
    double y;
    double z;
};

class Matrix
{
public:
    double x_11;
    double x_12;
    double x_13;
    double x_21;
    double x_22;
    double x_23;
    double x_31;
    double x_32;
    double x_33;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton__remove_clicked();

    void on_pushButton_coplanar_clicked();

private:
    Ui::Widget *ui;
    int m_number_of_points;
    bool are_three_points_on_one_line( Point p1, Point p2, Point p3  );
    bool are_all_points_on_one_line( Point* p, int size_p, int* ind_1, int* ind_2, int* ind_3 );
    double determinant( Matrix m );
    bool are_all_points_complanar( Point* p, int size_p );

};

#endif // WIDGET_H
