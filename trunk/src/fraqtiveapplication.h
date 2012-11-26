/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2009 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef FRAQTIVEAPPLICATION_H
#define FRAQTIVEAPPLICATION_H

#include <QApplication>
#include <QPointer>

class JobScheduler;
class ConfigurationData;
class FraqtiveMainWindow;
class AboutBox;
class GuideDialog;

class FraqtiveApplication : public QApplication
{
    Q_OBJECT
public:
    FraqtiveApplication( int& argc, char** argv );
    ~FraqtiveApplication();

public:
    JobScheduler* jobScheduler() const { return m_jobScheduler; }

    ConfigurationData* configuration() const { return m_configuration; }

public slots:
    void about();
    void showQuickGuide();
    void openDonations();

private:
    QString version() const;

    QString technicalInformation();

private:
    JobScheduler* m_jobScheduler;
    ConfigurationData* m_configuration;
    FraqtiveMainWindow* m_mainWindow;

    QPointer<AboutBox> m_aboutBox;
    QPointer<GuideDialog> m_guideDialog;
};

inline FraqtiveApplication* fraqtive()
{
    return static_cast<FraqtiveApplication*>( qApp );
}

#endif
