/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <utilities/units/Quantity.hpp>

#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/ScaleFactory.hpp>
#include <utilities/units/IPUnit.hpp>
#include <utilities/units/IPUnit_Impl.hpp>
#include <utilities/units/TemperatureUnit.hpp>
#include <utilities/units/TemperatureUnit_Impl.hpp>

#include <utilities/math/FloatCompare.hpp>

#include <utilities/core/Assert.hpp>

#include <cmath>

int _QuantityType = qRegisterMetaType<openstudio::Quantity>("openstudio::Quantity");

namespace openstudio{

Quantity::Quantity(const UnitSystem& system)
  : m_value(0.0), m_units(createDimensionlessUnit(system))
{}

Quantity::Quantity(double value, const UnitSystem& system)
  : m_value(value), m_units(createDimensionlessUnit(system))
{}

Quantity::Quantity(double value,const Unit& units)
  : m_value(value), m_units(units.clone())
{}

Quantity::Quantity(const Quantity &q)
  : m_value(q.value()), m_units(q.units())
{}

Quantity& Quantity::operator=(const Quantity& q) {
  if (this == &q) {
    return *this;
  }

  m_value = q.value();
  m_units = q.units();
  return *this;
}

double Quantity::value() const { 
  return m_value;
}

void Quantity::setValue(double newValue) {
  m_value = newValue;
}

Unit Quantity::units() const {
  return m_units.clone();
}

UnitSystem Quantity::system() const {
  return m_units.system();
}

bool Quantity::isTemperature() const {
  return m_units.optionalCast<TemperatureUnit>();
}

std::vector<std::string> Quantity::baseUnits() const {
  return m_units.baseUnits();
}

int Quantity::baseUnitExponent(const std::string& baseUnit) const {
  return m_units.baseUnitExponent(baseUnit);
}

void Quantity::setBaseUnitExponent( const std::string& baseUnit, int exponent) {
  m_units.setBaseUnitExponent( baseUnit, exponent );
}

std::string Quantity::standardUnitsString(bool withScale) const {
  return m_units.standardString(withScale);
}

std::string Quantity::prettyUnitsString(bool withScale) const {
  return m_units.prettyString(withScale);
}

void Quantity::setPrettyUnitsString( const std::string& str ) {
  return m_units.setPrettyString( str );
}

const Scale& Quantity::scale() const {
  return m_units.scale();
}

bool Quantity::setScale(int scaleExponent) {
  ScaleConstant candidate = ScaleFactory::instance().createScale(scaleExponent);
  double candidateValue = candidate().value;
  if (candidateValue == 0.0) {
    return false;
  }
  m_value = m_value * (scale().value / candidateValue);
  bool ok = m_units.setScale(scaleExponent);
  OS_ASSERT(ok);
  return true;
}

bool Quantity::setScale(const std::string& scaleAbbreviation) {
  ScaleConstant candidate = ScaleFactory::instance().createScale(scaleAbbreviation);
  double candidateValue = candidate().value;
  if (candidateValue == 0.0) {
    return false;
  }
  m_value = m_value * (scale().value / candidateValue);
  bool ok = m_units.setScale(scaleAbbreviation);
  OS_ASSERT(ok);
  return true;
}

bool Quantity::isAbsolute() const {
  OptionalTemperatureUnit tu = m_units.optionalCast<TemperatureUnit>();
  if(!tu) {
    LOG_AND_THROW("Could not evaluate Quantity::isAbsolute for quantity " << *this
        << " because it is in system " << system().valueName() << ", not Celsius or Fahrenheit.");
  }
  return tu->isAbsolute();
}

bool Quantity::isRelative() const {
  return !isAbsolute();
}

void Quantity::setAsAbsolute() {
  OptionalTemperatureUnit tu = m_units.optionalCast<TemperatureUnit>();
  if(!tu) {
    LOG_AND_THROW("Could not Quantity::setAsAbsolute for quantity " << *this
        << " because it is in system " << system().valueName() << ", not Celsius or Fahrenheit.");
  }
  tu->setAsAbsolute();
}

void Quantity::setAsRelative() {
  OptionalTemperatureUnit tu = m_units.optionalCast<TemperatureUnit>();
  if(!tu) {
    LOG_AND_THROW("Could not Quantity::setAsRelative for quantity " << *this
        << " because it is in system " << system().valueName() << ", not Celsius or Fahrenheit.");
  }
  tu->setAsRelative();
}

void Quantity::lbmToLbf() 
{
  OptionalIPUnit iu = m_units.optionalCast<IPUnit>();
  if(!iu) {
    LOG_AND_THROW("Cannot convert non-IP quantity " << *this << " in system "
        << system().valueName() << " from pound-mass to pound-force.");
  }

  // convert from (value * lb_m^x * ...) to (value * lb_f^x * ...)
  // lb_m^x * (1/gc)^x = lb_f^x * s^{2x} / ft^x
  int x = baseUnitExponent("lb_m");
  if (x != 0) {
    iu->lbmToLbf();
    m_value /= std::pow(IPUnit::gc(),x);
  }
  OS_ASSERT(baseUnitExponent("lb_m") == 0);
}

void Quantity::lbfToLbm() 
{
  OptionalIPUnit iu = m_units.optionalCast<IPUnit>();
  if(!iu) {
    LOG_AND_THROW("Cannot convert non-IP quantity " << *this << " in system "
        << system().valueName() << " from pound-force to pound-mass.");
  }

  // convert from (value * lb_f^x * ...) to (value * lb_m^x * ...)
  // lb_f^x * gc^x = lb_m^x * ft^x / s^{2x}
  int x = baseUnitExponent("lb_f");
  if (x != 0) {
    iu->lbfToLbm();
    m_value *= std::pow(IPUnit::gc(),x);
  }
  OS_ASSERT(baseUnitExponent("lb_f") == 0);
}

Quantity& Quantity::operator+=(const Quantity& rQuantity) {

  if (this == &rQuantity) {
    m_value *= 2.0;
    return *this;
  }

  if (m_units != rQuantity.m_units) {
    LOG_AND_THROW("Cannot add quantities with different units.");
  }

  if (scale() != rQuantity.scale()) {
    Quantity wRQuantity(rQuantity);
    wRQuantity.setScale(scale().exponent);
    m_value += wRQuantity.value();
  }
  else {
    m_value += rQuantity.value();
  }

  if (isTemperature() && rQuantity.isTemperature()) {
    if (!isAbsolute() && rQuantity.isAbsolute()) {
      setAsAbsolute();
    }
  }

  return *this;
}

Quantity& Quantity::operator-=(const Quantity& rQuantity) {

  if (this == &rQuantity) {
    m_value = 0.0;
    return *this;
  }

  if (m_units != rQuantity.m_units) {
    LOG_AND_THROW("Cannot subtract quantities with different units.");
  }

  if (scale() != rQuantity.scale()) {
    Quantity wRQuantity(rQuantity);
    wRQuantity.setScale(scale().exponent);
    m_value -= wRQuantity.value();
  }
  else {
    m_value -= rQuantity.value();
  }

  if (isTemperature() && rQuantity.isTemperature()) {
    if (isAbsolute() && rQuantity.isAbsolute()) {
      // units must be the same, check that exponent on this is 1
      std::vector<std::string> bus = baseUnits();
      assert(bus.size() == 1);
      if (baseUnitExponent(bus[0]) == 1) {
        setAsRelative();
      }
    }
    else if (!isAbsolute() && rQuantity.isAbsolute()) {
      setAsAbsolute();
    }
  }

  return *this;
}

Quantity& Quantity::operator*=(const Quantity& rQuantity) {

  ScaleOpReturnType resultScale;
  if (this == &rQuantity) {
    m_value = std::pow(m_value,2);
    m_units.pow(2);
    resultScale = openstudio::pow(scale(),2);
  }
  else {
    // unit *= will throw if m_units is not base-class and rQuantity.m_units is different system
    if ((system() != rQuantity.system()) && (system() != UnitSystem::Mixed)) {
      m_units = m_units.cloneToMixed();
    }
    // ETH@20100517 ugly workaround for temperature units
    OptionalTemperatureUnit tu1 = m_units.optionalCast<TemperatureUnit>();
    OptionalTemperatureUnit tu2 = rQuantity.m_units.optionalCast<TemperatureUnit>();
    if (tu1 && tu2) {
      *tu1 *= *tu2;
    }
    else {
      m_units *= rQuantity.m_units;
    }
    m_value *= rQuantity.value();
    resultScale = scale()*rQuantity.scale();
  }
  m_value *= resultScale.second;

  return *this;
}

Quantity& Quantity::operator/=(const Quantity& rQuantity) {
  if (this == &rQuantity) {
    // result = 1
    m_units /= m_units;
    m_value = 1.0;
  }
  else {
    // unit /= will throw if m_units is not base-class and rQuantity.m_units is different system
    if ((system() != rQuantity.system()) && (system() != UnitSystem::Mixed)) {
      m_units = m_units.cloneToMixed();
    }
    // ETH@20100517 ugly workaround for temperature units
    OptionalTemperatureUnit tu1 = m_units.optionalCast<TemperatureUnit>();
    OptionalTemperatureUnit tu2 = rQuantity.m_units.optionalCast<TemperatureUnit>();
    if (tu1 && tu2) {
      *tu1 /= *tu2;
    }
    else {
      m_units /= rQuantity.m_units;
    }
    m_value /= rQuantity.value();
    ScaleOpReturnType resultScale = scale()/rQuantity.scale();
    m_value *= resultScale.second;
  }

  return *this;
}

Quantity& Quantity::operator*=(double d) {
  m_value *= d;
  return *this;
}

Quantity& Quantity::operator/=(double d) {
  m_value /= d;
  return *this;
}

Quantity& Quantity::pow(int expNum,int expDenom) {
  // try unit first--may throw
  m_units.pow(expNum,expDenom);
  ScaleOpReturnType resultScale = openstudio::pow(scale(),expNum,expDenom);
  m_value = std::pow(m_value,double(expNum)/double(expDenom));
  m_value = m_value*resultScale.second;

  return *this;
}

std::ostream& operator<<(std::ostream& os,const Quantity& q) {
  os << q.value();
  std::stringstream unitString;
  unitString << q.m_units;
  if (unitString.str() != "") {
    os << " " << unitString.str();
  }
  return os;
}

Quantity operator-(const Quantity& rQuantity) {
  Quantity result(rQuantity);
  result.setValue(-result.value());
  return result;
}

Quantity operator+(const Quantity& lQuantity,const Quantity& rQuantity) {
  Quantity result(lQuantity);
  result += rQuantity;
  return result;
}

Quantity operator-(const Quantity& lQuantity,const Quantity& rQuantity) {
  Quantity result(lQuantity);
  result -= rQuantity;
  return result;
}

Quantity operator*(const Quantity& lQuantity,const Quantity& rQuantity) {
  Quantity result(lQuantity);
  result *= rQuantity;
  return result;
}

Quantity operator/(const Quantity& lQuantity,const Quantity& rQuantity) {
  Quantity result(lQuantity);
  result /= rQuantity;
  return result;
}

Quantity pow(const Quantity& rQuantity,int expNum,int expDenom) {
  Quantity result(rQuantity);
  result.pow(expNum,expDenom);
  return result;
}

Quantity operator*(const Quantity& lQuantity,double d) {
  Quantity result(lQuantity);
  result *= d;
  return result;
}

Quantity operator*(double d,const Quantity& rQuantity) {
  return rQuantity*d;
}

Quantity operator/(const Quantity& lQuantity,double d) {
  Quantity result(lQuantity);
  result /= d;
  return result;
}

Quantity operator/(double d,const Quantity& rQuantity) {
  Quantity result(d,rQuantity.system());
  result /= rQuantity;
  return result;
}

bool operator==(const Quantity& lQuantity,const Quantity& rQuantity) {
  if ((lQuantity.system() == rQuantity.system()) && (lQuantity.units() == rQuantity.units())) {
    Quantity wl(lQuantity), wr(rQuantity);
    wl.setScale(0);
    wr.setScale(0);
    return equal(wl.value(),wr.value());
  }
  return false;
}

bool operator!=(const Quantity& lQuantity,const Quantity& rQuantity) {
  return !(lQuantity == rQuantity);
}

} // openstudio
