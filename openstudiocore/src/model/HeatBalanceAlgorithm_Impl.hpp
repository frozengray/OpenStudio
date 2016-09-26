/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_HEATBALANCEALGORITHM_IMPL_HPP
#define MODEL_HEATBALANCEALGORITHM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** HeatBalanceAlgorithm_Impl is a ModelObject_Impl that is the implementation class for HeatBalanceAlgorithm.*/
  class MODEL_API HeatBalanceAlgorithm_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string algorithm READ algorithm WRITE setAlgorithm RESET resetAlgorithm);
    Q_PROPERTY(bool isAlgorithmDefaulted READ isAlgorithmDefaulted);
    Q_PROPERTY(std::vector<std::string> algorithmValues READ algorithmValues);

    Q_PROPERTY(double surfaceTemperatureUpperLimit READ surfaceTemperatureUpperLimit WRITE setSurfaceTemperatureUpperLimit RESET resetSurfaceTemperatureUpperLimit);
    Q_PROPERTY(openstudio::Quantity surfaceTemperatureUpperLimit_SI READ surfaceTemperatureUpperLimit_SI WRITE setSurfaceTemperatureUpperLimit RESET resetSurfaceTemperatureUpperLimit);
    Q_PROPERTY(openstudio::Quantity surfaceTemperatureUpperLimit_IP READ surfaceTemperatureUpperLimit_IP WRITE setSurfaceTemperatureUpperLimit RESET resetSurfaceTemperatureUpperLimit);
    Q_PROPERTY(bool isSurfaceTemperatureUpperLimitDefaulted READ isSurfaceTemperatureUpperLimitDefaulted);

    Q_PROPERTY(double minimumSurfaceConvectionHeatTransferCoefficientValue READ minimumSurfaceConvectionHeatTransferCoefficientValue WRITE setMinimumSurfaceConvectionHeatTransferCoefficientValue RESET resetMinimumSurfaceConvectionHeatTransferCoefficientValue);
    Q_PROPERTY(openstudio::Quantity minimumSurfaceConvectionHeatTransferCoefficientValue_SI READ minimumSurfaceConvectionHeatTransferCoefficientValue_SI WRITE setMinimumSurfaceConvectionHeatTransferCoefficientValue RESET resetMinimumSurfaceConvectionHeatTransferCoefficientValue);
    Q_PROPERTY(openstudio::Quantity minimumSurfaceConvectionHeatTransferCoefficientValue_IP READ minimumSurfaceConvectionHeatTransferCoefficientValue_IP WRITE setMinimumSurfaceConvectionHeatTransferCoefficientValue RESET resetMinimumSurfaceConvectionHeatTransferCoefficientValue);
    Q_PROPERTY(bool isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted READ isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted);

    Q_PROPERTY(double maximumSurfaceConvectionHeatTransferCoefficientValue READ maximumSurfaceConvectionHeatTransferCoefficientValue WRITE setMaximumSurfaceConvectionHeatTransferCoefficientValue RESET resetMaximumSurfaceConvectionHeatTransferCoefficientValue);
    Q_PROPERTY(openstudio::Quantity maximumSurfaceConvectionHeatTransferCoefficientValue_SI READ maximumSurfaceConvectionHeatTransferCoefficientValue_SI WRITE setMaximumSurfaceConvectionHeatTransferCoefficientValue RESET resetMaximumSurfaceConvectionHeatTransferCoefficientValue);
    Q_PROPERTY(openstudio::Quantity maximumSurfaceConvectionHeatTransferCoefficientValue_IP READ maximumSurfaceConvectionHeatTransferCoefficientValue_IP WRITE setMaximumSurfaceConvectionHeatTransferCoefficientValue RESET resetMaximumSurfaceConvectionHeatTransferCoefficientValue);
    Q_PROPERTY(bool isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted READ isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    HeatBalanceAlgorithm_Impl(const IdfObject& idfObject,
                              Model_Impl* model,
                              bool keepHandle);

    // construct from workspace
    HeatBalanceAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    // clone copy constructor
    HeatBalanceAlgorithm_Impl(const HeatBalanceAlgorithm_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    // virtual destructor
    virtual ~HeatBalanceAlgorithm_Impl() {}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const override;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent) override;

    //@}

    /** @name Virtual Methods */
    //@{

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    std::string algorithm() const;

    bool isAlgorithmDefaulted() const;

    double surfaceTemperatureUpperLimit() const;

    Quantity getSurfaceTemperatureUpperLimit(bool returnIP=false) const;

    bool isSurfaceTemperatureUpperLimitDefaulted() const;

    double minimumSurfaceConvectionHeatTransferCoefficientValue() const;

    Quantity getMinimumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP=false) const;

    bool isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

    double maximumSurfaceConvectionHeatTransferCoefficientValue() const;

    Quantity getMaximumSurfaceConvectionHeatTransferCoefficientValue(bool returnIP=false) const;

    bool isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAlgorithm(std::string algorithm);

    void resetAlgorithm();

    bool setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit);

    bool setSurfaceTemperatureUpperLimit(const Quantity& surfaceTemperatureUpperLimit);

    void resetSurfaceTemperatureUpperLimit();

    bool setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue);

    bool setMinimumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& minimumSurfaceConvectionHeatTransferCoefficientValue);

    void resetMinimumSurfaceConvectionHeatTransferCoefficientValue();

    bool setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue);

    bool setMaximumSurfaceConvectionHeatTransferCoefficientValue(const Quantity& maximumSurfaceConvectionHeatTransferCoefficientValue);

    void resetMaximumSurfaceConvectionHeatTransferCoefficientValue();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HeatBalanceAlgorithm");

    std::vector<std::string> algorithmValues() const;
    openstudio::Quantity surfaceTemperatureUpperLimit_SI() const;
    openstudio::Quantity surfaceTemperatureUpperLimit_IP() const;
    openstudio::Quantity minimumSurfaceConvectionHeatTransferCoefficientValue_SI() const;
    openstudio::Quantity minimumSurfaceConvectionHeatTransferCoefficientValue_IP() const;
    openstudio::Quantity maximumSurfaceConvectionHeatTransferCoefficientValue_SI() const;
    openstudio::Quantity maximumSurfaceConvectionHeatTransferCoefficientValue_IP() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HEATBALANCEALGORITHM_IMPL_HPP

