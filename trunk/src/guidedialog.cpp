/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2012 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "guidedialog.h"

#include "iconloader.h"
#include "xmlui/gradientwidget.h"
#include "xmlui/builder.h"
#include "xmlui/toolstrip.h"

#include <QBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QDialogButtonBox>
#include <QPushButton>

GuideDialog::GuideDialog( QWidget* parent ) : QDialog( parent )
{
    setAttribute( Qt::WA_DeleteOnClose, true );

    setWindowTitle( tr( "Quick Guide" ) );

    QAction* action;

    action = new QAction( IconLoader::icon( "back" ), tr( "Back" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( goBack() ) );
    setAction( "goBack", action );

    action = new QAction( IconLoader::icon( "forward" ), tr( "Forward" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( goForward() ) );
    setAction( "goForward", action );

    action = new QAction( IconLoader::icon( "home" ), tr( "Home" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( goHome() ) );
    setAction( "goHome", action );

    loadXmlUiFile( ":/resources/guidedialog.xml" );

    XmlUi::Builder* builder = new XmlUi::Builder( this );
    builder->addClient( this );

    QVBoxLayout* topLayout = new QVBoxLayout( this );
    topLayout->setMargin( 0 );
    topLayout->setSpacing( 0 );

    XmlUi::GradientWidget* promptWidget = new XmlUi::GradientWidget( this );
    topLayout->addWidget( promptWidget );

    QHBoxLayout* promptLayout = new QHBoxLayout( promptWidget );
    promptLayout->setSpacing( 10 );

    QLabel* promptPixmap = new QLabel( promptWidget );
    promptPixmap->setPixmap( IconLoader::pixmap( "help", 22 ) );
    promptLayout->addWidget( promptPixmap );

    QLabel* promptLabel = new QLabel( promptWidget );
    promptLabel->setWordWrap( true );
    promptLabel->setText( tr( "Quick guide to Fraqtive:" ) );
    promptLabel->setMinimumWidth( 350 );
    promptLabel->setFixedHeight( promptLabel->heightForWidth( 350 ) );
    promptLayout->addWidget( promptLabel, 1 );

    QFrame* separator = new QFrame( this );
    separator->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    topLayout->addWidget( separator );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setMargin( 9 );
    mainLayout->setSpacing( 4 );
    topLayout->addLayout( mainLayout );

    XmlUi::ToolStrip* strip = new XmlUi::ToolStrip( this );
    builder->registerToolStrip( "stripGuide", strip );
    mainLayout->addWidget( strip );

    m_browser = new QTextBrowser( this );
    m_browser->setOpenExternalLinks( true );
    mainLayout->addWidget( m_browser, 1 );

    connect( m_browser, SIGNAL( historyChanged() ), this, SLOT( updateActions() ) );

    mainLayout->addSpacing( 7 );

    QDialogButtonBox* buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok, Qt::Horizontal, this );
    mainLayout->addWidget( buttonBox );

    buttonBox->button( QDialogButtonBox::Ok )->setText( tr( "&OK" ) );

    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );

    updateActions();

    goHome();
}

GuideDialog::~GuideDialog()
{
}

void GuideDialog::goBack()
{
    m_browser->backward();
}

void GuideDialog::goForward()
{
    m_browser->forward();
}

void GuideDialog::goHome()
{
    m_browser->setSource( QUrl::fromLocalFile( ":/tutorial/index.html" ) );
}

void GuideDialog::updateActions()
{
    action( "goBack" )->setEnabled( m_browser->isBackwardAvailable() );
    action( "goForward" )->setEnabled( m_browser->isForwardAvailable() );
}
