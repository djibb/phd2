/*
 *  guider_onestar.h
 *  PHD Guiding
 *
 *  Created by Craig Stark.
 *  Copyright (c) 2006-2010 Craig Stark.
 *  All rights reserved.
 *
 *  Refactored by Bret McKee
 *  Copyright (c) 2012 Bret McKee
 *  All rights reserved.
 *
 *  This source code is distributed under the following "BSD" license
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *    Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *    Neither the name of Bret McKee, Dad Dog Development,
 *     Craig Stark, Stark Labs nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef GUIDER_ONESTAR_H_INCLUDED
#define GUIDER_ONESTAR_H_INCLUDED

class MassChecker;
class GuiderOneStar;
class GuiderConfigDialogCtrlSet;

class GuiderOneStarConfigDialogCtrlSet : public GuiderConfigDialogCtrlSet
{

public:
    GuiderOneStarConfigDialogCtrlSet(wxWindow *pParent, Guider *pGuider, AdvancedDialog *pAdvancedDialog, BrainCtrlIdMap& CtrlMap);
    virtual ~GuiderOneStarConfigDialogCtrlSet();

    GuiderOneStar *m_pGuiderOneStar;
    wxSpinCtrl *m_pSearchRegion;
    wxCheckBox *m_pEnableStarMassChangeThresh;
    wxCheckBox *m_pEnableGuideOnSlit;
    wxSpinCtrlDouble *m_pMassChangeThreshold;
    wxSpinCtrlDouble *m_MinHFD;

    virtual void LoadValues();
    virtual void UnloadValues();
    void OnStarMassEnableChecked(wxCommandEvent& event);
    void OnGuideOnSlitChecked(wxCommandEvent& event);
};

class GuiderOneStar : public Guider
{
private:
    Star m_star;
    MassChecker *m_massChecker;

    // parameters
    bool m_massChangeThresholdEnabled;
    bool m_guideOnSlitEnabled ;
    double m_massChangeThreshold;
    bool m_tolerateJumpsEnabled;
    double m_tolerateJumpsThreshold;

public:
    class GuiderOneStarConfigDialogPane : public GuiderConfigDialogPane
    {
    protected:

        public:
        GuiderOneStarConfigDialogPane(wxWindow *pParent, GuiderOneStar *pGuider);
        ~GuiderOneStarConfigDialogPane() {};

        virtual void LoadValues() {};
        virtual void UnloadValues() {};
        void LayoutControls(Guider *pGuider, BrainCtrlIdMap& CtrlMap);
    };

    bool GetMassChangeThresholdEnabled() const;
    double GetMassChangeThreshold() const;
    bool GetGuideOnSlitEnabled() const;
    void SetMassChangeThresholdEnabled(bool enable);
    void SetGuideOnSlitEnabled(bool enable);
    
    bool SetMassChangeThreshold(double starMassChangeThreshold);
    bool SetTolerateJumps(bool enable, double threshold);
    bool SetSearchRegion(int searchRegion);

    friend class GuiderOneStarConfigDialogPane;
    friend class GuiderOneStarConfigDialogCtrlSet;

public:
    GuiderOneStar(wxWindow *parent);
    virtual ~GuiderOneStar();

    void OnPaint(wxPaintEvent& evt) override;

    bool IsLocked() override;
    bool AutoSelect() override;
    const PHD_Point& CurrentPosition() override;
    wxRect GetBoundingBox() override;
    int GetMaxMovePixels() override;
    double StarMass() override;
    unsigned int StarPeakADU() override;
    double SNR() override;
    double HFD() override;
    int StarError() override;
    wxString GetSettingsSummary() const override;

    Guider::GuiderConfigDialogPane *GetConfigDialogPane(wxWindow *pParent) override;
    GuiderConfigDialogCtrlSet *GetConfigDialogCtrlSet(wxWindow *pParent, Guider *pGuider, AdvancedDialog *pAdvancedDialog, BrainCtrlIdMap& CtrlMap) override;

    void LoadProfileSettings();

private:
    bool IsValidLockPosition(const PHD_Point& pt) final;
    void InvalidateCurrentPosition(bool fullReset = false) final;
    bool UpdateCurrentPosition(const usImage *pImage, GuiderOffset *ofs, FrameDroppedInfo *errorInfo) final;
    bool SetCurrentPosition(const usImage *pImage, const PHD_Point& position) final;

    void OnLClick(wxMouseEvent& evt);

    void SaveStarFITS();

    DECLARE_EVENT_TABLE()
};

#endif /* GUIDER_ONESTAR_H_INCLUDED */
