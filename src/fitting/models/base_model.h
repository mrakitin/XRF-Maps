/***
Copyright (c) 2016, UChicago Argonne, LLC. All rights reserved.

Copyright 2016. UChicago Argonne, LLC. This software was produced
under U.S. Government contract DE-AC02-06CH11357 for Argonne National
Laboratory (ANL), which is operated by UChicago Argonne, LLC for the
U.S. Department of Energy. The U.S. Government has rights to use,
reproduce, and distribute this software.  NEITHER THE GOVERNMENT NOR
UChicago Argonne, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is
modified to produce derivative works, such modified software should
be clearly marked, so as not to confuse it with the version available
from ANL.

Additionally, redistribution and use in source and binary forms, with
or without modification, are permitted provided that the following
conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.

    * Neither the name of UChicago Argonne, LLC, Argonne National
      Laboratory, ANL, the U.S. Government, nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY UChicago Argonne, LLC AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL UChicago
Argonne, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
***/

/// Initial Author <2016>: Arthur Glowacki


#ifndef Base_Model_H
#define Base_Model_H

#include "data_struct/spectra.h"
#include "data_struct/fit_parameters.h"
#include "data_struct/fit_element_map.h"

namespace fitting
{
namespace models
{

using namespace data_struct;
using namespace std;

/*
 1 batch_a: matrix batch fit
 2 batch_b: batch_fit_wo_tails
 3 batch_c: batch_fit_w_tails
 4 batch fit with free E, everything else fixed
 */
enum class Fit_Params_Preset { MATRIX_BATCH_FIT, BATCH_FIT_NO_TAILS, BATCH_FIT_WITH_TAILS, BATCH_FIT_WITH_FREE_ENERGY };


/**
 * @brief The Base_Model class: base class for modeling spectra and fitting elements
 */
class DLL_EXPORT Base_Model
{
public:
    /**
     * @brief Base_Model : Constructor
     */
    Base_Model();

    /**
     * @brief ~Base_Model : Destructor
     */
    ~Base_Model();

    /**
     * @brief fit_parameters : returns Fit_Parameters class of the required fit parameters to run a fitting
     * @return
     */
    virtual const Fit_Parameters& fit_parameters() const = 0;

    /**
     * @brief model_spectrum : Model a spectra based on the fit parameters passed in.
     * @param fit_params : Fitting parameters required to model the spectra.
     * @param spectra : Might be depricated when I remove the snip background function
     * @param calibration : Energy calibration
     * @param elements_to_fit : List of elemetns to use in modeling the spectra.
     * @param energy_range : Spectra model energy range. Basically the size of the spectra model returned;
     * @return
     */
    virtual const Spectra model_spectrum(const Fit_Parameters * const fit_params,
                                         const Fit_Element_Map_Dict * const elements_to_fit,
                                         const struct Range energy_range) = 0;


    virtual const Spectra model_spectrum_element(const Fit_Parameters * const fitp,
                                                 const Fit_Element_Map * const element_to_fit,
                                                 const ArrayXr &ev) = 0;

    virtual const ArrayXr peak(real_t gain, real_t sigma, const ArrayXr& delta_energy) const = 0;

    virtual const ArrayXr step(real_t gain, real_t sigma, const ArrayXr& delta_energy, real_t peak_E) const = 0;

    virtual const ArrayXr tail(real_t gain, real_t sigma, const ArrayXr& delta_energy, real_t gamma) const = 0;

    virtual const ArrayXr elastic_peak(const Fit_Parameters * const fitp, const ArrayXr& ev, real_t gain) const = 0;

    virtual const ArrayXr compton_peak(const Fit_Parameters * const fitp, const ArrayXr& ev, real_t gain) const = 0;

    virtual void reset_to_default_fit_params() = 0;

    virtual void update_fit_params_values(Fit_Parameters *fit_params) = 0;

protected:



private:


};

} //namespace models

} //namespace fitting

#endif // Base_Model_H
