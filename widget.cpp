/*
This program allow to find out if given points are coplanar

Copyright (C) <2016>  <Elena Filenko>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QDoubleValidator>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_number_of_points = 0;

    QDoubleValidator* double_val = new QDoubleValidator( 0.00, 1000.00, 2, this );
    double_val->setNotation( QDoubleValidator::StandardNotation );
    this->ui->lineEdit_X->setValidator( double_val );
    this->ui->lineEdit_Y->setValidator( double_val );
    this->ui->lineEdit_Z->setValidator( double_val );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_add_clicked()
{
    if( ( this->ui->lineEdit_X->text().compare( QString( "" ) ) != 0 ) &&
        ( this->ui->lineEdit_Y->text().compare( QString( "" ) ) != 0 ) &&
        ( this->ui->lineEdit_Z->text().compare( QString( "" ) ) != 0 ) )
    {
        QTableWidgetItem* x = new QTableWidgetItem( this->ui->lineEdit_X->text() );
        this->ui->tableWidget->setItem( m_number_of_points, 0, x );

        QTableWidgetItem* y = new QTableWidgetItem( this->ui->lineEdit_Y->text() );
        this->ui->tableWidget->setItem( m_number_of_points, 1, y );


        QTableWidgetItem* z = new QTableWidgetItem( this->ui->lineEdit_Z->text() );
        this->ui->tableWidget->setItem( m_number_of_points, 2, z );

        ++m_number_of_points;
    }
}


void Widget::on_pushButton__remove_clicked()
{
    int row = this->ui->tableWidget->currentRow();
    this->ui->tableWidget->removeRow( row );
    --m_number_of_points;
}

void Widget::on_pushButton_coplanar_clicked()
{

    Point* p =  new Point[ m_number_of_points ];

    for( int i = 0; i < m_number_of_points; i++ )
    {
        p[ i ].x = this->ui->tableWidget->item( i, 0 )->text().toDouble();
        p[ i ].y = this->ui->tableWidget->item( i, 1 )->text().toDouble();
        p[ i ].z = this->ui->tableWidget->item( i, 2 )->text().toDouble();

        qDebug() << "i = " << i <<
                    " p[ i ].x = " << p[ i ].x <<
                    " p[ i ].y = " << p[ i ].y <<
                    " p[ i ].z = " << p[ i ].z;
    }

    bool res = are_all_points_complanar( p, m_number_of_points );

    if( res )
        this->ui->lineEdit_result->setText( QString( "COPLANAR") );
    else
        this->ui->lineEdit_result->setText( QString( "NOT COPLANAR") );
    delete [] p;
}


bool Widget::are_three_points_on_one_line( Point p1, Point p2, Point p3  )
{
    bool res = false;
    double distance_p1_p2 = sqrt( (p1.x - p2.x)*(p1.x - p2.x ) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z) );
    double distance_p1_p3 = sqrt( (p1.x - p3.x)*(p1.x - p3.x ) + (p1.y - p3.y)*(p1.y - p3.y) + (p1.z - p3.z)*(p1.z - p3.z) );
    double distance_p2_p3 = sqrt( (p2.x - p3.x)*(p2.x - p3.x ) + (p2.y - p3.y)*(p2.y - p3.y) + (p2.z - p3.z)*(p2.z - p3.z) );

    //std::cout << distance_p1_p2 << " " << distance_p1_p3 << " " << distance_p2_p3 << endl;
    if( abs( distance_p1_p2 + distance_p1_p3 - distance_p2_p3 ) < 0.01 )
        res = true;

    if( abs( distance_p1_p2 + distance_p2_p3 - distance_p1_p3 ) < 0.01 )
        res = true;

    if( abs( distance_p2_p3 + distance_p1_p3 - distance_p1_p2 ) < 0.01 )
        res = true;

    return res;
}

bool Widget::are_all_points_on_one_line( Point* p, int size_p, int* ind_1, int* ind_2, int* ind_3 )
{
    bool res = true;

    for( int i=0; i < size_p; i++ )
    {
        for( int j=i+1; j < size_p; j++ )
        {
            for( int k = j+1; k < size_p; k++ )
            {
                if( ! are_three_points_on_one_line( p[i], p[j], p[k] ) )
                {
                    *ind_1 = i;
                    *ind_2 = j;
                    *ind_3 = k;
                    res = false;

                    i = size_p;
                    j = size_p;
                    k = size_p;
                }
            }
        }
    }

    return res;
}

double Widget::determinant( Matrix m )
{
    double res = m.x_11*m.x_22*m.x_33 + m.x_31*m.x_12*m.x_23 + m.x_21*m.x_32*m.x_13 -
              m.x_13*m.x_22*m.x_31 - m.x_12*m.x_21*m.x_33 - m.x_11*m.x_32*m.x_23;

    return res;
}


bool Widget::are_all_points_complanar( Point* p, int size_p )
{
    bool res = true;
    int ind_1;
    int ind_2;
    int ind_3;
    Matrix m;

    bool on_one_line = are_all_points_on_one_line( p, size_p, &ind_1, &ind_2, &ind_3 );

    if( !on_one_line )
    {
        for( int i = 0; i < size_p; i++ )
        {
            m.x_11 = p[ i ].x - p[ ind_1 ].x;
            m.x_12 = p[ i ].y - p[ ind_1 ].y;
            m.x_13 = p[ i ].z - p[ ind_1 ].z;

            m.x_21 = p[ ind_2 ].x - p[ ind_1 ].x;
            m.x_22 = p[ ind_2 ].y - p[ ind_1 ].y;
            m.x_23 = p[ ind_2 ].z - p[ ind_1 ].z;

            m.x_31 = p[ ind_3 ].x - p[ ind_1 ].x;
            m.x_32 = p[ ind_3 ].y - p[ ind_1 ].y;
            m.x_33 = p[ ind_3 ].z - p[ ind_1 ].z;

            double det = determinant( m );

            if( det )
            {
                res = false;
                break;
            }
        }
    }
    return res;
}
