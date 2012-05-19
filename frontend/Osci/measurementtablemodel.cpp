/*

SampleScope: A humble USB oscilloscope
Copyright (C) 2012 mru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "measurementtablemodel.h"

MeasurementTableModel::MeasurementTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    for (int i = 0; i <2; i ++ ) {
        for ( int j = 0; j < Meas_END; j++ ) {
            createIndex(i,j, i*100+j);
        }
    }
}

static QVariant checkNaN(double n) {
    if (n == std::numeric_limits<double>::quiet_NaN())
        return "n/a";
    return n;
}

QVariant MeasurementTableModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case Qt::DisplayRole: {
        return items[index.column()][index.row()];
    }
    break;

    // for reference, the rest of the enumeration:
    case Qt::DecorationRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
    case Qt::StatusTipRole:
    case Qt::WhatsThisRole:
    case Qt::SizeHintRole:
    case Qt::FontRole:
    case Qt::TextAlignmentRole:
    case Qt::BackgroundRole:
        //case Qt::BackgroundColorRole:
    case Qt::ForegroundRole:
        //case Qt::TextColorRole:
    case Qt::CheckStateRole:
    default:
        return QVariant::Invalid;
    }
    return QVariant::Invalid;
}

int MeasurementTableModel::rowCount(const QModelIndex &/*parent*/) const {
    return Meas_END;
}

int MeasurementTableModel::columnCount(const QModelIndex &/*parent*/) const {
    return 2;
}

QVariant MeasurementTableModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    QVariant data = QVariant::Invalid;

    const char* horiz[] = {
        "Ch1", "Ch2"
    };

    const char* vert[] = {
        "rms [div]",
        "rms [V]",
        "avg [div]",
        "avg [V]",
        //"f fft [Hz]",
        //"f acorr [Hz]",
        "F sin [Hz]"
    };

    const char* tooltip[] = {
        "RMS Value [div]",
        "RMS Value [Volt]",
        "Average value [div]",
        "Average value [Volt]",
        //"Frequency [Hz], calculated with FFT",
        //"Frequency [Hz], calculated with auto correlation",
        "Frequency [Hz], calculated with fitted Sine"
    };

    switch (orientation) {
    case Qt::Vertical:
        switch (role) {
        case Qt::DisplayRole:
            data = vert[section];
            break;
        case Qt::ToolTipRole:
            data = tooltip[section];
            break;
        default:
            data = QVariant::Invalid;
            break;
        }
        break;
    case Qt::Horizontal:
        switch (role) {
        case Qt::DisplayRole:
            data = horiz[section];
            break;
        default:
            data = QVariant::Invalid;
        }
        break;
    default:
        data = QVariant::Invalid;
    }

    return data;

#if 0
    switch (role) {
    case Qt::DisplayRole:
        return data;
        break;
    case Qt::ToolTipRole:
        return tooltip[section];
        break;
    case Qt::DecorationRole:
    case Qt::EditRole:
    case Qt::StatusTipRole:
    case Qt::WhatsThisRole:
    case Qt::SizeHintRole:
    case Qt::FontRole:
    case Qt::TextAlignmentRole:
    case Qt::BackgroundRole:
        //case Qt::BackgroundColorRole:
    case Qt::ForegroundRole:
        //case Qt::TextColorRole:
    case Qt::CheckStateRole:
    default:
        return QVariant::Invalid;
    }
    return QVariant::Invalid;
#endif
}


void MeasurementTableModel::setData(const Measurement& meas) {
    for (int i = 0; i < 2; i ++ ) {
        const Measurement::ChannelData& ch = meas.getChannel((DeviceConstants::Channel_t) i);
        items[i][Meas_RMS_div] = checkNaN(ch.rms_div);
        items[i][Meas_RMS_Volt] = checkNaN(ch.rms_volt);
        items[i][Meas_AVG_div] = checkNaN(ch.avg_div);
        items[i][Meas_AVG_Volt] = checkNaN(ch.avg_volt);
        //items[i][Meas_Freq_FFT] = checkNaN(ch.fpeak);
        //items[i][Meas_Freq_ACORR] = checkNaN(ch.apeak);
        items[i][Meas_Freq_Sin] = checkNaN(ch.speak);
    }

    const QModelIndex& i0 = index(0,0);
    const QModelIndex& i1 = index(rowCount(QModelIndex())-1, columnCount(QModelIndex())-1);

    emit dataChanged(i0,i1);
}
