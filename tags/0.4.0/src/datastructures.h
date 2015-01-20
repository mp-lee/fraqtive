/**************************************************************************
* This file is part of the Fraqtive program
* Copyright (C) 2004-2008 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QPolygonF>
#include <QMetaType>

#include "generatorcore.h"

enum Fractal
{
    MandelbrotFractal,
    JuliaFractal
};

enum ExponentType
{
    IntegralExponent,
    RealExponent
};

class FractalType
{
public:
    FractalType();

public:
    void setFractal( Fractal fractal ) { m_fractal = fractal; }
    Fractal fractal() const { return m_fractal; }

    void setParameter( const QPointF& param ) { m_parameter = param; }
    QPointF parameter() const { return m_fractal == JuliaFractal ? m_parameter : QPointF(); }

    void setExponentType( ExponentType type ) { m_exponentType = type; }
    ExponentType exponentType() const { return m_exponentType; }

    void setIntegralExponent( int exponent ) { m_integralExponent = exponent; }
    int integralExponent() const { return m_exponentType == IntegralExponent ? m_integralExponent : 0; }

    void setRealExponent( double exponent ) { m_realExponent = exponent; }
    double realExponent() const { return m_exponentType == RealExponent ? m_realExponent : 0.0; }

    void setVariant( GeneratorCore::Variant variant ) { m_variant = variant; }
    GeneratorCore::Variant variant() const { return m_variant; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const FractalType& type );
    friend QDataStream& operator >>( QDataStream& stream, FractalType& type );

    friend bool operator ==( const FractalType& lhv, const FractalType& rhv );
    friend bool operator !=( const FractalType& lhv, const FractalType& rhv ) { return !( lhv == rhv ); }

private:
    Fractal m_fractal;
    QPointF m_parameter;

    ExponentType m_exponentType;
    int m_integralExponent;
    double m_realExponent;

    GeneratorCore::Variant m_variant;
};

inline FractalType::FractalType() :
    m_fractal( MandelbrotFractal ),
    m_exponentType( IntegralExponent ),
    m_integralExponent( 0 ),
    m_realExponent( 0.0 ),
    m_variant( GeneratorCore::NormalVariant )
{
}

inline bool operator ==( const FractalType& lhv, const FractalType& rhv )
{
    return ( lhv.m_fractal == rhv.m_fractal )
        && ( lhv.m_fractal != JuliaFractal || lhv.m_parameter.x() == rhv.m_parameter.x() && lhv.m_parameter.y() == rhv.m_parameter.y() ) // exact compare
        && ( lhv.m_exponentType == rhv.m_exponentType )
        && ( lhv.m_exponentType != IntegralExponent || lhv.m_integralExponent == rhv.m_integralExponent )
        && ( lhv.m_exponentType != RealExponent || qFuzzyCompare( lhv.m_realExponent, rhv.m_realExponent ) )
        && ( lhv.m_variant == rhv.m_variant );
}

Q_DECLARE_METATYPE( FractalType )

class Position
{
public:
    Position();

public:
    void setCenter( const QPointF& center ) { m_center = center; }
    QPointF center() const { return m_center; }

    void setZoomFactor( double factor ) { m_zoomFactor = factor; }
    double zoomFactor() const { return m_zoomFactor; }

    void setAngle( double angle ) { m_angle = angle; }
    double angle() const { return m_angle; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const Position& position );
    friend QDataStream& operator >>( QDataStream& stream, Position& position );

    friend bool operator ==( const Position& lhv, const Position& rhv );
    friend bool operator !=( const Position& lhv, const Position& rhv ) { return !( lhv == rhv ); }

private:
    QPointF m_center;
    double m_zoomFactor; // log10
    double m_angle; // degrees
};

inline Position::Position() :
    m_zoomFactor( 0.0 ),
    m_angle( 0.0 )
{
}

inline bool operator ==( const Position& lhv, const Position& rhv )
{
    return ( lhv.m_center.x() == rhv.m_center.x() && lhv.m_center.y() == rhv.m_center.y() ) // exact compare
        && qFuzzyCompare( lhv.m_zoomFactor, rhv.m_zoomFactor )
        && qFuzzyCompare( lhv.m_angle, rhv.m_angle );
}

Q_DECLARE_METATYPE( Position )

class Gradient
{
public:
    Gradient() { }
    Gradient( const QPolygonF& red, const QPolygonF& green, const QPolygonF& blue );

public:
    void setRed( const QPolygonF& red ) { m_red = red; }
    QPolygonF red() const { return m_red; }

    void setGreen( const QPolygonF& green ) { m_green = green; }
    QPolygonF green() const { return m_green; }

    void setBlue( const QPolygonF& blue ) { m_blue = blue; }
    QPolygonF blue() const { return m_blue; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const Gradient& gradient );
    friend QDataStream& operator >>( QDataStream& stream, Gradient& gradient );

    friend bool operator ==( const Gradient& lhv, const Gradient& rhv );
    friend bool operator !=( const Gradient& lhv, const Gradient& rhv ) { return !( lhv == rhv ); }

private:
    QPolygonF m_red;
    QPolygonF m_green;
    QPolygonF m_blue;
};

inline Gradient::Gradient( const QPolygonF& red, const QPolygonF& green, const QPolygonF& blue ) :
    m_red( red ),
    m_green( green ),
    m_blue( blue )
{
}

inline bool operator ==( const Gradient& lhv, const Gradient& rhv )
{
    return ( lhv.m_red == rhv.m_red )
        && ( lhv.m_green == rhv.m_green )
        && ( lhv.m_blue == rhv.m_blue );
}

Q_DECLARE_METATYPE( Gradient )

class ColorMapping
{
public:
    ColorMapping();

public:
    void setMirrored( bool mirrored ) { m_mirrored = mirrored; }
    bool isMirrored() const { return m_mirrored; }

    void setReversed( bool reversed ) { m_reversed = reversed; }
    bool isReversed() const { return m_reversed; }

    void setScale( double scale ) { m_scale = scale; }
    double scale() const { return m_scale; }

    void setOffset( double offset ) { m_offset = offset; }
    double offset() const { return m_offset; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const ColorMapping& mapping );
    friend QDataStream& operator >>( QDataStream& stream, ColorMapping& mapping );

    friend bool operator ==( const ColorMapping& lhv, const ColorMapping& rhv );
    friend bool operator !=( const ColorMapping& lhv, const ColorMapping& rhv ) { return !( lhv == rhv ); }

private:
    bool m_mirrored;
    bool m_reversed;
    double m_scale;
    double m_offset;
};

inline ColorMapping::ColorMapping() :
    m_mirrored( false ),
    m_reversed( false ),
    m_scale( 0.0 ),
    m_offset( 0.0 )
{
}

inline bool operator ==( const ColorMapping& lhv, const ColorMapping& rhv )
{
    return ( lhv.m_mirrored == rhv.m_mirrored )
        && ( lhv.m_reversed == rhv.m_reversed )
        && qFuzzyCompare( lhv.m_scale, rhv.m_scale )
        && qFuzzyCompare( lhv.m_offset, rhv.m_offset );
}

Q_DECLARE_METATYPE( ColorMapping )

class GeneratorSettings
{
public:
    GeneratorSettings();

public:
    void setCalculationDepth( double depth ) { m_calculationDepth = depth; }
    double calculationDepth() const { return m_calculationDepth; }

    void setDetailThreshold( double threshold ) { m_detailThreshold = threshold; }
    double detailThreshold() const { return m_detailThreshold; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const GeneratorSettings& settings );
    friend QDataStream& operator >>( QDataStream& stream, GeneratorSettings& settings );

    friend bool operator ==( const GeneratorSettings& lhv, const GeneratorSettings& rhv );
    friend bool operator !=( const GeneratorSettings& lhv, const GeneratorSettings& rhv ) { return !( lhv == rhv ); }

private:
    double m_calculationDepth;
    double m_detailThreshold;
};

inline GeneratorSettings::GeneratorSettings() :
    m_calculationDepth( 0.0 ),
    m_detailThreshold( 0.0 )
{
}

inline bool operator ==( const GeneratorSettings& lhv, const GeneratorSettings& rhv )
{
    return qFuzzyCompare( lhv.m_calculationDepth, rhv.m_calculationDepth )
        && qFuzzyCompare( lhv.m_detailThreshold, rhv.m_detailThreshold );
}

Q_DECLARE_METATYPE( GeneratorSettings )

enum AntiAliasing
{
    NoAntiAliasing,
    LowAntiAliasing,
    MediumAntiAliasing,
    HighAntiAliasing
};

class ViewSettings
{
public:
    ViewSettings();

public:
    void setAntiAliasing( AntiAliasing antiAliasing ) { m_antiAliasing = antiAliasing; }
    AntiAliasing antiAliasing() const { return m_antiAliasing; }

public:
    friend QDataStream& operator <<( QDataStream& stream, const ViewSettings& settings );
    friend QDataStream& operator >>( QDataStream& stream, ViewSettings& settings );

    friend bool operator ==( const ViewSettings& lhv, const ViewSettings& rhv );
    friend bool operator !=( const ViewSettings& lhv, const ViewSettings& rhv ) { return !( lhv == rhv ); }

private:
    AntiAliasing m_antiAliasing;
};

inline ViewSettings::ViewSettings() :
    m_antiAliasing( NoAntiAliasing )
{
}

inline bool operator ==( const ViewSettings& lhv, const ViewSettings& rhv )
{
    return lhv.m_antiAliasing == rhv.m_antiAliasing;
}

Q_DECLARE_METATYPE( ViewSettings )

void registerDataStructures();

#endif