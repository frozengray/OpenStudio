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

#include <gtest/gtest.h>

#include <utilities/idd/Test/IddFixture.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddFieldProperties.hpp>
#include <utilities/idd/IddKey.hpp>

#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/Quantity.hpp>

#include <utilities/core/Containers.hpp>
#include <utilities/core/Compare.hpp>

#include <OpenStudio.hxx>

#include <boost/foreach.hpp>

using namespace openstudio;

TEST_F(IddFixture,IddFactory_Version_Header) {
  ASSERT_NO_THROW(IddFactory::instance().getVersion(IddFileType::OpenStudio));
  ASSERT_NO_THROW(IddFactory::instance().getVersion(IddFileType::EnergyPlus));
  EXPECT_ANY_THROW(IddFactory::instance().getVersion(IddFileType::WholeFactory));
  EXPECT_ANY_THROW(IddFactory::instance().getVersion(IddFileType::UserCustom));

  // OpenStudio always has major, minor, patch
  VersionString osVersion(IddFactory::instance().getVersion(IddFileType::OpenStudio));
  EXPECT_TRUE(osVersion.patch());
  EXPECT_FALSE(osVersion.build());
  EXPECT_EQ(VersionString(openStudioVersion()),osVersion);
  EXPECT_TRUE(osVersion.fidelityEqual(VersionString(openStudioVersion())));

  // EnergyPlus always has major, minor, patch and build
  VersionString epVersion(IddFactory::instance().getVersion(IddFileType::EnergyPlus));
  EXPECT_TRUE(epVersion.patch());
  EXPECT_TRUE(epVersion.build());
  EXPECT_EQ(VersionString(energyPlusVersion()),epVersion);
  EXPECT_FALSE(epVersion.fidelityEqual(VersionString(energyPlusVersion())));

  EXPECT_NO_THROW(IddFactory::instance().getHeader(IddFileType::OpenStudio));
  EXPECT_NO_THROW(IddFactory::instance().getHeader(IddFileType::EnergyPlus));
  EXPECT_ANY_THROW(IddFactory::instance().getHeader(IddFileType::WholeFactory));
  EXPECT_ANY_THROW(IddFactory::instance().getHeader(IddFileType::UserCustom));
}

TEST_F(IddFixture,IddFactory_IddFile)
{
  IddFile file = IddFactory::instance().getIddFile(IddFileType::OpenStudio);
  EXPECT_TRUE(file.getObject("OS:Building"));
  EXPECT_TRUE(file.getObject(IddObjectType::CommentOnly));
  EXPECT_TRUE(file.getObject(IddObjectType::OS_ClimateZones));
  // print out for visual inspection
  path outPath = resourcesPath()/toPath("utilities/RoundTrip_OpenStudio.idd");
  file.save(outPath,true);
  // check against getIddObjects
  IddObjectVector objects = IddFactory::instance().getObjects(IddFileType::OpenStudio);
  EXPECT_TRUE(file.objects().size() == objects.size());
  // required objects
  EXPECT_TRUE(file.requiredObjects().size() > 0);
  EXPECT_TRUE(file.requiredObjects().size() == 
              IddFactory::instance().getRequiredObjects(IddFileType::OpenStudio).size());
  EXPECT_TRUE(file.requiredObjects().size() <= IddFactory::instance().requiredObjects().size());
  // unique objects
  EXPECT_TRUE(file.uniqueObjects().size() > 0);
  EXPECT_TRUE(file.uniqueObjects().size() == 
              IddFactory::instance().getUniqueObjects(IddFileType::OpenStudio).size());
  EXPECT_TRUE(file.uniqueObjects().size() <= IddFactory::instance().uniqueObjects().size());

  file = IddFactory::instance().getIddFile(IddFileType::EnergyPlus);
  EXPECT_TRUE(file.getObject("Building"));
  EXPECT_TRUE(file.getObject(IddObjectType::CommentOnly));
  EXPECT_FALSE(file.getObject(IddObjectType::OS_ClimateZones));
  // print out for visual inspection
  outPath = resourcesPath()/toPath("energyplus/RoundTrip_ProposedEnergy+.idd");
  file.save(outPath,true);
  // check against getIddObjects
  objects = IddFactory::instance().getObjects(IddFileType::EnergyPlus);
  EXPECT_TRUE(file.objects().size() == objects.size());
}

TEST_F(IddFixture,IddFactory_isInFile)
{
  EXPECT_TRUE(IddFactory::instance().isInFile(IddObjectType::Building,IddFileType::EnergyPlus));
  EXPECT_FALSE(IddFactory::instance().isInFile(IddObjectType::Building,IddFileType::OpenStudio));
  EXPECT_FALSE(IddFactory::instance().isInFile(IddObjectType::OS_Material_AirWall,IddFileType::EnergyPlus));
  EXPECT_TRUE(IddFactory::instance().isInFile(IddObjectType::OS_Material_AirWall,IddFileType::OpenStudio));
  EXPECT_TRUE(IddFactory::instance().isInFile(IddObjectType::GlobalGeometryRules,IddFileType::EnergyPlus));
  EXPECT_FALSE(IddFactory::instance().isInFile(IddObjectType::GlobalGeometryRules,IddFileType::OpenStudio));
  EXPECT_TRUE(IddFactory::instance().isInFile(IddObjectType::CommentOnly,IddFileType::EnergyPlus));
  EXPECT_TRUE(IddFactory::instance().isInFile(IddObjectType::CommentOnly,IddFileType::OpenStudio));
  EXPECT_FALSE(IddFactory::instance().isInFile(IddObjectType::Catchall,IddFileType::EnergyPlus));
  EXPECT_FALSE(IddFactory::instance().isInFile(IddObjectType::Catchall,IddFileType::OpenStudio));
}

TEST_F(IddFixture,IddFactory_IddObjects)
{
  IddObject object;

  OptionalIddObject candidate = IddFactory::instance().getObject(IddObjectType::UserCustom);
  EXPECT_FALSE(candidate);

  candidate = IddFactory::instance().getObject(IddObjectType::Catchall);
  ASSERT_TRUE(candidate);
  object = *candidate;
  candidate = IddFactory::instance().getObject("catchall");
  ASSERT_TRUE(candidate);
  EXPECT_TRUE(object == *candidate);

  candidate = IddFactory::instance().getObject(IddObjectType::CommentOnly);
  ASSERT_TRUE(candidate);
  object = *candidate;
  candidate = IddFactory::instance().getObject("coMmentonLy");
  ASSERT_TRUE(candidate);
  EXPECT_TRUE(object == *candidate);

  candidate = IddFactory::instance().getObject(IddObjectType::Lights);
  ASSERT_TRUE(candidate);
  object = *candidate;
  candidate = IddFactory::instance().getObject("lights");
  ASSERT_TRUE(candidate);
  EXPECT_TRUE(object == *candidate);
  EXPECT_EQ("Lights", object.name());
  ASSERT_TRUE(object.nonextensibleFields().size() > static_cast<unsigned>(0));
  EXPECT_EQ("Return Air Fraction Function of Plenum Temperature Coefficient 2",
            object.nonextensibleFields().back().name());

  OptionalIddField field = object.getField(0);
  ASSERT_TRUE(field);
  EXPECT_EQ("Name",field->name());
  EXPECT_TRUE(field->properties().required);
  EXPECT_EQ(IddFieldType(IddFieldType::AlphaType),field->properties().type);
  ASSERT_TRUE(!field->properties().references.empty());
  EXPECT_EQ(static_cast<unsigned>(1), field->properties().references.size());
  EXPECT_EQ("LightsNames", *(field->properties().references.begin()));
  EXPECT_FALSE(field->properties().stringDefault); // OptionalString should be false--no default name
  EXPECT_EQ(IddFieldProperties::Unbounded,field->properties().minBoundType);

  ASSERT_TRUE(object.nonextensibleFields().size() > static_cast<unsigned>(3));
  field = object.getField(3);
  EXPECT_EQ("Design Level Calculation Method",field->name());
  EXPECT_FALSE(field->properties().required);
  EXPECT_TRUE(field->properties().stringDefault);
  EXPECT_EQ("LightingLevel",*(field->properties().stringDefault));
  EXPECT_EQ(IddFieldType(IddFieldType::ChoiceType),field->properties().type);
  EXPECT_EQ(static_cast<unsigned>(3),field->keys().size());
}

// ETH@20100521 Using this test to locate objects with characteristics I am looking for. Would
// rather use Ruby, but not quite sure about getting/using the installer.
TEST_F(IddFixture,IddFactory_ObjectFinder) {
  IddFileType fileType = IddFileType::WholeFactory;
  struct DesiredCharacteristics {
    bool operator()(const IddObject& object) {
      // fields that are \autocalculatable with no default, or a default other than 'autocalculate',
      // or fields that are \autosizable with no default or a default other than 'autosize'.
      BOOST_FOREACH(const IddField& iddField, object.nonextensibleFields()) {
        if (iddField.properties().autocalculatable && (!iddField.properties().stringDefault ||
            !istringEqual("autocalculate",iddField.properties().stringDefault.get()))) 
        {
          return true;
        }
        if (iddField.properties().autosizable && (!iddField.properties().stringDefault ||
            !istringEqual("autosize",iddField.properties().stringDefault.get()))) 
        {
          return true;
        }
      }
      return false;
    }
  };

  IddFile iddFile = IddFactory::instance().getIddFile(fileType);
  IddObjectVector objects = iddFile.objects();
  LOG(Info,"IddObjects that satisfy DesiredCharacteristics:");
  DesiredCharacteristics tester;
  BOOST_FOREACH(const IddObject& object,objects) {
    if (tester(object)) {
      LOG(Info,"  " << object.name());
    }
  }
}

TEST_F(IddFixture,IddFactory_RoomAir_TemperaturePattern_TwoGradient) {
  // there seems to be a problem with this object
  OptionalIddObject object = IddFactory::instance().getObject("RoomAir:TemperaturePattern:TwoGradient");
  ASSERT_TRUE(object);
  ASSERT_TRUE(object->type() == IddObjectType::RoomAir_TemperaturePattern_TwoGradient);
}

TEST_F(IddFixture,IddFactory_Units) {
  std::vector<boost::regex> unsupported;
  unsupported.push_back(boost::regex("\\$"));
  unsupported.push_back(boost::regex("eV"));
  unsupported.push_back(boost::regex("hh:mm"));
  unsupported.push_back(boost::regex("percent"));
  unsupported.push_back(boost::regex("ppm"));

  IddObjectVector objects = IddFactory::instance().getObjects(IddFileType(IddFileType::WholeFactory));
  StringSet goodUnits;
  StringSet badUnits;
  BOOST_FOREACH(const IddObject& object,objects) {
    IddFieldVector fields = object.nonextensibleFields();
    IddFieldVector temp = object.extensibleGroup();
    fields.insert(fields.end(),temp.begin(),temp.end());
    BOOST_FOREACH(const IddField& field,fields) {
      OptionalString iddUnits = field.properties().units;
      OptionalUnit siUnit;
      if (iddUnits) {

        // check if already tested units
        if (goodUnits.find(*iddUnits) != goodUnits.end()){
          continue;
        }
        if (badUnits.find(*iddUnits) != badUnits.end()){
          continue;
        }

        // screen for unsupported units
        BOOST_FOREACH(const boost::regex& re,unsupported) {
          if (boost::regex_search(*iddUnits,re)) {
            iddUnits = boost::none;
            break;
          }
        }
        if (!iddUnits) {
          continue;
        }

        siUnit = field.getUnits(false);
        EXPECT_TRUE(siUnit || field.unitsBasedOnOtherField());
        if (siUnit) {
          // could just return junk unit. if not junk, quantity will be convertable
          // to UnitSystem::SI.
          Quantity q(1.0,*siUnit);
          OptionalQuantity testQ = convert(q,UnitSystem(UnitSystem::SI));
          EXPECT_TRUE(testQ);
          if (testQ) {
            goodUnits.insert(*iddUnits);
            EXPECT_TRUE(testQ->system() == UnitSystem::SI);
          } else {
            badUnits.insert(*iddUnits);
            LOG(Debug,"Unable to convert unit '" << *iddUnits << "' to SI for field '"
                << field.name() << "' in IddObject '" << object.name() << "'.");
          }
        }
        else if (field.unitsBasedOnOtherField()) {
          goodUnits.insert(*iddUnits);
          continue;
        }
        else {
          badUnits.insert(*iddUnits);
          LOG(Debug,"Unable to instantiate unit '" << *iddUnits << "' for field '"
              << field.name() << "' in IddObject '" << object.name() << "'.");
          continue;
        }
        OptionalUnit ipUnit = field.getUnits(true);
        EXPECT_TRUE(ipUnit);
        if (ipUnit) {
          // could just return junk unit. if not junk, quantity will be convertable
          // to *siUnit or UnitSystem::SI.
          Quantity q(1.0,*ipUnit);
          OptionalQuantity testQ;
          if (siUnit) {
            testQ = convert(q,*siUnit);
          }
          else {
            testQ = convert(q,UnitSystem(UnitSystem::SI));
          }
          EXPECT_TRUE(testQ);
          if (testQ) {
            goodUnits.insert(*iddUnits);
          }
          else {
            badUnits.insert(ipUnit->standardString());
            LOG(Debug,"Unable to convert unit " << *ipUnit << " to IDD/SI units " << *siUnit 
                << " for field '" << field.name() << "' in IddObject '" << object.name() << "'.");
          }
        }
        else {
          badUnits.insert(*iddUnits);
          LOG(Debug,"Unable to instantiate ipUnit for field " << field.name() << " in IddObject " 
              << object.name() << ", which has units " << *siUnit << ".");
        }
      }
    }
  }
  LOG(Info,"IddUnitStrings not handled properly by the Factories and Converter:");
  BOOST_FOREACH(const std::string& str,badUnits) {
    LOG(Info,"  " << str);
  }
  LOG(Info,"IddUnitStrings handled properly by the Factories and Converter:");
  BOOST_FOREACH(const std::string& str,goodUnits) {
    LOG(Info,"  " << str);
  }
}

TEST_F(IddFixture,IddFactory_OpenStudioUnits_SubTest1) {
  IddObjectVector objects = IddFactory::instance().getObjects(IddFileType(IddFileType::OpenStudio));
  bool found = false;
  BOOST_FOREACH(const IddObject& object,objects) {
    IddFieldVector fields = object.nonextensibleFields();
    IddFieldVector temp = object.extensibleGroup();
    fields.insert(fields.end(),temp.begin(),temp.end());
    BOOST_FOREACH(const IddField& field,fields) {
      if (OptionalString iddUnits = field.properties().ipUnits) {
        if (*iddUnits == "Btu/h-ft-F") {
          found = true;
          OptionalUnit siUnit = field.getUnits(false);
          ASSERT_TRUE(siUnit);
          EXPECT_TRUE(siUnit->system() == UnitSystem::SI);
          EXPECT_EQ("kg*m/s^3*K",siUnit->standardString(false));
          OptionalUnit ipUnit = field.getUnits(true);
          ASSERT_TRUE(ipUnit);
          EXPECT_TRUE(ipUnit->system() == UnitSystem::BTU);
          Quantity q(1.0,*ipUnit);
          OptionalQuantity testQ = convert(q,*siUnit);
          EXPECT_TRUE(testQ);
          break;
        }
      }
    }
    if (found) {
      break;
    }
  }
  EXPECT_TRUE(found);
}

TEST_F(IddFixture,IddFactory_OpenStudioUnits_SubTest2) {
  IddObjectVector objects = IddFactory::instance().getObjects(IddFileType(IddFileType::OpenStudio));
  bool found = false;
  BOOST_FOREACH(const IddObject& object,objects) {
    IddFieldVector fields = object.nonextensibleFields();
    IddFieldVector temp = object.extensibleGroup();
    fields.insert(fields.end(),temp.begin(),temp.end());
    BOOST_FOREACH(const IddField& field,fields) {
      if (OptionalString iddUnits = field.properties().units) {
        if (*iddUnits == "kg-H2O/kg-Air") {
          found = true;
          OptionalUnit siUnit = field.getUnits();
          ASSERT_TRUE(siUnit);
          EXPECT_TRUE(siUnit->system() == UnitSystem::SI);
          EXPECT_EQ("",siUnit->standardString());
          EXPECT_EQ("kg_{H2O}/kg_{air}",siUnit->prettyString());
          break;
        }
      }  
    }
    if (found) {
      break;
    }
  }
  EXPECT_TRUE(found);
}

TEST_F(IddFixture,IddFactory_OpenStudioUnits_SubTest3) {
  IddObjectVector objects = IddFactory::instance().getObjects(IddFileType(IddFileType::OpenStudio));
  bool found = false;
  BOOST_FOREACH(const IddObject& object,objects) {
    IddFieldVector fields = object.nonextensibleFields();
    IddFieldVector temp = object.extensibleGroup();
    fields.insert(fields.end(),temp.begin(),temp.end());
    BOOST_FOREACH(const IddField& field,fields) {
      if (OptionalString iddUnits = field.properties().ipUnits) {
        if (*iddUnits == "gal/min") {
          if (object.type() == IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat) {
            LOG(Debug,"Gal/min field: " << field.name());
          }
          found = true;
          OptionalUnit ipUnit = field.getUnits(true);
          ASSERT_TRUE(ipUnit);
          EXPECT_TRUE(ipUnit->system() == UnitSystem::Mixed);
          EXPECT_EQ("crgal^3/min",ipUnit->standardString());
          EXPECT_EQ("gal/min",ipUnit->prettyString());
        }
      }  
    }
  }
  EXPECT_TRUE(found);
}
