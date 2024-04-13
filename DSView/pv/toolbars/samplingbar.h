/*
 * This file is part of the DSView project.
 * DSView is based on PulseView.
 *
 * Copyright (C) 2013 DreamSourceLab <support@dreamsourcelab.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef DSVIEW_PV_TOOLBARS_SAMPLINGBAR_H
#define DSVIEW_PV_TOOLBARS_SAMPLINGBAR_H

#include "../sigsession.h"

#include <stdint.h>
#include <list>
#include <map>
  
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include "../ui/dscombobox.h"

struct st_dev_inst;
class QAction;

namespace pv
{ 
    class SigSession;

    namespace device
    {
        class DevInst;
    } 

    namespace dialogs
    {
        class deviceoptions;
        class Calibration;
    }

    namespace toolbars
    {

        class SamplingBar : public QToolBar
        {
            Q_OBJECT

        private:
            static const int ComboBoxMaxWidth = 200;
            static const int RefreshShort = 500;
            static const uint64_t LogicMaxSWDepth64 = SR_GB(16);
            static const uint64_t LogicMaxSWDepth32 = SR_GB(8);
            
            static const uint64_t AnalogMaxSWDepth = SR_Mn(100);
            static const QString RLEString;
            static const QString DIVString;
            static const uint64_t ZeroTimeBase = SR_US(2);

        public:
            SamplingBar(SigSession *session, QWidget *parent);
            void set_device_list(const std::list<DevInst*> &devices, DevInst* selected);
            DevInst *get_selected_device();
            void update_sample_rate_selector();

            void set_sampling(bool sampling);
            bool get_sampling();
            bool get_instant();

            void enable_toggle(bool enable);
            void enable_run_stop(bool enable);
            void enable_instant(bool enable);

            double hori_knob(int dir);
            double commit_hori_res();
            double get_hori_res();

        public slots:
            void set_sample_rate(uint64_t sample_rate);

        signals:
            void sig_run_stop();
            void sig_instant_stop();
            void sig_device_selected();
            void sig_device_updated();
            void sig_duration_changed();
            void sig_show_calibration();
            void sig_hide_calibration();

        private:
            void changeEvent(QEvent *event);
            void retranslateUi();
            void reStyle();

            void update_sample_rate_selector_value();
            void update_sample_count_selector();
            void update_sample_count_selector_value();
            void commit_settings();
            void setting_adj();

        private slots:
            void on_mode();
            void on_run_stop();
            void on_instant_stop();
            void on_device_selected();
            void on_samplerate_sel(int index);
            void on_samplecount_sel(int index);
            void ctrl_start();
            void ctrl_stop();
            void ctrl_instant();

            void show_session_error(
                const QString text, const QString info_text);

        public slots:
            void on_configure();
            void zero_adj();
            void reload();

        private:
            SigSession          *_session;
            mutable std::mutex  _sampling_mutex;
            bool                _enable;
            bool                _sampling;

            QToolButton         _device_type;
            DsComboBox           _device_selector;
            std::map<const void *, DevInst*> _device_selector_map;
            bool                _updating_device_selector;

            QToolButton         _configure_button;
            DsComboBox           _sample_count;
            DsComboBox           _sample_rate;
            bool                _updating_sample_rate;
            bool                _updating_sample_count;

            QToolButton         _run_stop_button;
            QToolButton         _instant_button;
            QAction             *_run_stop_action;
            QAction             *_instant_action;
            QAction             *_mode_action;
            QToolButton         _mode_button;

            QMenu               *_mode_menu;
            QAction             *_action_repeat;
            QAction             *_action_single;
            bool                _instant;
        };

    } // namespace toolbars
} // namespace pv

#endif // DSVIEW_PV_TOOLBARS_SAMPLINGBAR_H
