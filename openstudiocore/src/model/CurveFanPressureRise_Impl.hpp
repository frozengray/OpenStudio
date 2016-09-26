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

#ifndef MODEL_CURVEFANPRESSURERISE_IMPL_HPP
#define MODEL_CURVEFANPRESSURERISE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveFanPressureRise_Impl is a Curve_Impl that is the implementation class for CurveFanPressureRise.*/
  class MODEL_API CurveFanPressureRise_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(double coefficient1C1 READ coefficient1C1 WRITE setCoefficient1C1);
    Q_PROPERTY(double coefficient2C2 READ coefficient2C2 WRITE setCoefficient2C2);
    Q_PROPERTY(double coefficient3C3 READ coefficient3C3 WRITE setCoefficient3C3);
    Q_PROPERTY(double coefficient4C4 READ coefficient4C4 WRITE setCoefficient4C4);
    Q_PROPERTY(double minimumValueofQfan READ minimumValueofQfan WRITE setMinimumValueofQfan);
    Q_PROPERTY(double maximumValueofQfan READ maximumValueofQfan WRITE setMaximumValueofQfan);
    Q_PROPERTY(double minimumValueofPsm READ minimumValueofPsm WRITE setMinimumValueofPsm);
    Q_PROPERTY(double maximumValueofPsm READ maximumValueofPsm WRITE setMaximumValueofPsm);
    Q_PROPERTY(boost::optional<double> minimumCurveOutput READ minimumCurveOutput WRITE setMinimumCurveOutput RESET resetMinimumCurveOutput);
    Q_PROPERTY(boost::optional<double> maximumCurveOutput READ maximumCurveOutput WRITE setMaximumCurveOutput RESET resetMaximumCurveOutput);
   public:

    /** @name Constructors and Destructors */
    //@{

    CurveFanPressureRise_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveFanPressureRise_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    CurveFanPressureRise_Impl(const CurveFanPressureRise_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    virtual ~CurveFanPressureRise_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual int numVariables() const override;

    virtual double evaluate(const std::vector<double>& x) const override;

    //@}
    /** @name Getters */
    //@{

    double coefficient1C1() const;

    double coefficient2C2() const;

    double coefficient3C3() const;

    double coefficient4C4() const;

    double minimumValueofQfan() const;

    double maximumValueofQfan() const;

    double minimumValueofPsm() const;

    double maximumValueofPsm() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    //@}
    /** @name Setters */
    //@{

    void setCoefficient1C1(double coefficient1C1);

    void setCoefficient2C2(double coefficient2C2);

    void setCoefficient3C3(double coefficient3C3);

    void setCoefficient4C4(double coefficient4C4);

    void setMinimumValueofQfan(double minimumValueofQfan);

    void setMaximumValueofQfan(double maximumValueofQfan);

    void setMinimumValueofPsm(double minimumValueofPsm);

    void setMaximumValueofPsm(double maximumValueofPsm);

    void setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

    void resetMinimumCurveOutput();

    void setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

    void resetMaximumCurveOutput();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CurveFanPressureRise");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVEFANPRESSURERISE_IMPL_HPP

