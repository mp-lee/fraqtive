/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#include "fractalpresenter.h"

#include <math.h>

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#include <QMatrix>

#include "fractalgenerator.h"
#include "fractalmodel.h"
#include "fractaldata.h"
#include "abstractview.h"

FractalPresenter::FractalPresenter( QObject* parent ) : QObject( parent ),
    m_model( NULL ),
    m_view( NULL ),
    m_preview( false ),
    m_enabled( false )
{
    m_generator = new FractalGenerator( this );
    m_generator->setReceiver( this );
}

FractalPresenter::~FractalPresenter()
{
}

void FractalPresenter::setModel( FractalModel* model )
{
    m_model = model;
}

void FractalPresenter::setView( AbstractView* view )
{
    m_view = view;

    if ( m_model ) {
        m_view->setColorSettings( m_model->gradient(), m_model->backgroundColor(), m_model->colorMapping() );
        m_view->setViewSettings( m_model->viewSettings() );
    }
}

void FractalPresenter::setPreviewMode( bool preview )
{
    if ( m_preview != preview ) {
        m_preview = preview;
        m_generator->setPreviewMode( preview );
    }
}

void FractalPresenter::setPriority( int priority )
{
    m_generator->setPriority( priority );
}

void FractalPresenter::setEnabled( bool enabled )
{
    if ( m_enabled != enabled ) {
        m_enabled = enabled;
        m_generator->setEnabled( enabled );
        if ( !enabled )
            m_view->clearView();
        else
            m_view->initialUpdate( fractalData() );
    }
}

void FractalPresenter::setParameters( const FractalType& type, const Position& position )
{
    if ( m_type != type ) {
        m_type = type;
        m_position = position;
        m_generator->setParameters( type, position );
        if ( type.fractal() == JuliaFractal )
            clearHovering();
    } else {
        setPosition( position );
    }
}

void FractalPresenter::setFractalType( const FractalType& type )
{
    if ( m_type != type ) {
        m_type = type;
        m_generator->setFractalType( type );
        if ( type.fractal() == JuliaFractal )
            clearHovering();
    }
}

void FractalPresenter::setPosition( const Position& position )
{
    if ( m_position != position ) {
        if ( m_enabled && !m_preview ) {
            QMatrix oldMatrix = matrixFromPosition( m_position );
            QMatrix newMatrix = matrixFromPosition( position );
            QMatrix transform = ( newMatrix * oldMatrix.inverted() ).inverted();
            m_view->transformView( transform );
        }
        m_position = position;
        m_generator->setPosition( position );
    }
}

void FractalPresenter::setColorSettings( const Gradient& gradient, const QColor& background, const ColorMapping& mapping )
{
    m_view->setColorSettings( gradient, background, mapping );
}

void FractalPresenter::setGradient( const Gradient& gradient )
{
    m_view->setGradient( gradient );
}

void FractalPresenter::setBackgroundColor( const QColor& color )
{
    m_view->setBackgroundColor( color );
}

void FractalPresenter::setColorMapping( const ColorMapping& mapping )
{
    m_view->setColorMapping( mapping );
}

void FractalPresenter::setGeneratorSettings( const GeneratorSettings& settings )
{
    m_generator->setGeneratorSettings( settings );
}

void FractalPresenter::setViewSettings( const ViewSettings& settings )
{
    m_view->setViewSettings( settings );
}

const FractalData* FractalPresenter::fractalData()
{
    m_generator->updateData( &m_data );
    return &m_data;
}

int FractalPresenter::maximumIterations() const
{
    return m_generator->maximumIterations();
}

void FractalPresenter::setResolution( const QSize& resolution )
{
    m_generator->setResolution( resolution );
}

void FractalPresenter::setHoveringPoint( const QPointF& point )
{
    if ( m_model && m_model->fractalType().fractal() != JuliaFractal )
        m_model->setHoveringParameters( juliaType( point ), juliaPosition() );
}

void FractalPresenter::clearHovering()
{
    if ( m_model )
        m_model->clearHovering();
}

void FractalPresenter::setTrackingTransform( const QMatrix& transform )
{
    QMatrix oldMatrix = matrixFromPosition( m_position );
    QMatrix newMatrix = transform.inverted() * oldMatrix;

    Position position = positionFromMatrix( newMatrix );
    m_model->setTrackingPosition( position );
}

void FractalPresenter::clearTracking()
{
    m_model->clearTracking();
}

void FractalPresenter::changePosition( const QMatrix& transform )
{
    QMatrix oldMatrix = matrixFromPosition( m_position );
    QMatrix newMatrix = transform.inverted() * oldMatrix;

    Position position = positionFromMatrix( newMatrix );
    m_model->setPosition( position );
}

void FractalPresenter::switchToJulia( const QPointF& point )
{
    if ( m_model && m_model->fractalType().fractal() != JuliaFractal )
        m_model->setParameters( juliaType( point ), juliaPosition() );
}

void FractalPresenter::adjustCameraZoom( double delta )
{
    ViewSettings settings = m_model->viewSettings();

    double zoom = qBound( 10.0, settings.cameraZoom() - 3.5 * delta, 45.0 );

    settings.setCameraZoom( zoom );
    m_model->setViewSettings( settings );
}

void FractalPresenter::customEvent( QEvent* e )
{
    if ( e->type() == FractalGenerator::UpdateEvent && m_enabled ) {
        FractalGenerator::UpdateStatus status = m_generator->updateData( &m_data );
        switch ( status ) {
            case FractalGenerator::InitialUpdate:
                m_view->initialUpdate( &m_data );
                break;

            case FractalGenerator::PartialUpdate:
                m_view->partialUpdate( &m_data );
                break;

            case FractalGenerator::FullUpdate:
                m_view->fullUpdate( &m_data );
                break;
        }
    }
}

QMatrix FractalPresenter::matrixFromPosition( const Position& position )
{
    QSize resolution = m_data.size();
    QPointF center( resolution.width() / 2.0, resolution.height() / 2.0 );

    double scale = pow( 10.0, -position.zoomFactor() ) / resolution.height();

    QMatrix matrix;
    matrix.translate( position.center().x(), position.center().y() );
    matrix.rotate( -position.angle() );
    matrix.scale( scale, scale );
    matrix.translate( -center.x(), -center.y() );

    return matrix;
}

Position FractalPresenter::positionFromMatrix( const QMatrix& matrix )
{
    QSize resolution = m_data.size();
    QPointF center( resolution.width() / 2.0, resolution.height() / 2.0 );

    QLineF line( center, QPointF( center.x() + resolution.height(), center.y() ) );
    QLineF mapped = matrix.map( line );

    Position position;
    position.setCenter( mapped.p1() );
    position.setZoomFactor( -log10( mapped.length() ) );
    position.setAngle( -atan2( mapped.dy(), mapped.dx() ) * 180.0 / M_PI );

    if ( position.angle() < 0.0 )
        position.setAngle( position.angle() + 360.0 );

    return position;
}

FractalType FractalPresenter::juliaType( const QPointF& point )
{
    QMatrix matrix = matrixFromPosition( m_position );
    QPointF mapped = matrix.map( point );

    FractalType type = m_type;
    type.setFractal( JuliaFractal );
    type.setParameter( mapped );

    return type;
}

Position FractalPresenter::juliaPosition()
{
    double exponent = m_type.exponentType() == IntegralExponent ? m_type.integralExponent() : m_type.realExponent();
    double zoom = ( m_position.zoomFactor() + 0.45 ) / exponent - 0.45;

    Position position;
    position.setZoomFactor( zoom );

    return position;
}