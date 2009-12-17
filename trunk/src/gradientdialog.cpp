/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "gradientdialog.h"

#include <QPushButton>

#include "datastructures.h"

GradientDialog::GradientDialog( QWidget* parent ) : QDialog( parent )
{
    m_ui.setupUi( this );
}

GradientDialog::~GradientDialog()
{
}

void GradientDialog::setGradient( const Gradient& gradient )
{
    m_ui.gradient->setGradient( gradient );
    m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
}

Gradient GradientDialog::gradient() const
{
    return m_ui.gradient->gradient();
}

void GradientDialog::accept()
{
    emit applyGradient( gradient() );

    QDialog::accept();
}

void GradientDialog::on_gradient_gradientChanged()
{
    m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( true );
}

void GradientDialog::on_buttonBox_clicked( QAbstractButton* button )
{
    if ( button == m_ui.buttonBox->button( QDialogButtonBox::Apply ) ) {
        emit applyGradient( gradient() );
        m_ui.buttonBox->button( QDialogButtonBox::Apply )->setEnabled( false );
    }
}