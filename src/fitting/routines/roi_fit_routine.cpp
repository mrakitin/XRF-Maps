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



#include "roi_fit_routine.h"

namespace fitting
{
namespace routines
{

// --------------------------------------------------------------------------------------------------------------------

ROI_Fit_Routine::ROI_Fit_Routine() : Base_Fit_Routine()
{

}

// --------------------------------------------------------------------------------------------------------------------

ROI_Fit_Routine::~ROI_Fit_Routine()
{

}

// --------------------------------------------------------------------------------------------------------------------

 std::unordered_map<std::string, real_t> ROI_Fit_Routine::fit_spectra(const models::Base_Model * const model,
                                                                      const Spectra * const spectra,
                                                                      const Fit_Element_Map_Dict * const elements_to_fit)
{
    std::unordered_map<std::string, real_t> counts_dict;
    Fit_Parameters fitp = model->fit_parameters();
    unsigned int n_mca_channels = spectra->size();

    real_t energy_offset = fitp.value(STR_ENERGY_OFFSET);
    real_t energy_slope = fitp.value(STR_ENERGY_SLOPE);
    for(const auto& e_itr : *elements_to_fit)
    {
        unsigned int left_roi = 0;
        unsigned int right_roi = 0;
        Fit_Element_Map* element = e_itr.second;
        left_roi = (unsigned int)( ( (element->center() - element->width()) - energy_offset) / energy_slope);
        right_roi = (unsigned int)( ( (element->center() + element->width()) - energy_offset) / energy_slope);

        if (right_roi >= n_mca_channels)
        {
            right_roi = n_mca_channels - 2;
        }
        if (left_roi > right_roi)
        {
            left_roi = right_roi - 1;
        }

        //element->left_roi = left_roi;
        //element->right_roi = right_roi;

        size_t spec_size = (right_roi - left_roi) + 1;
        real_t counts = 0.0;
        counts = spectra->segment(left_roi, spec_size).sum();

        counts_dict[e_itr.first] = counts;
    }
    return counts_dict;
}

// --------------------------------------------------------------------------------------------------------------------

void ROI_Fit_Routine::initialize(models::Base_Model * const model,
                                 const Fit_Element_Map_Dict * const elements_to_fit,
                                 const struct Range energy_range)
{
    //N/A
}

// --------------------------------------------------------------------------------------------------------------------

} //namespace routines
} //namespace fitting
