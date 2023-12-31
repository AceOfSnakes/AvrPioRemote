/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "qstring.h"
#include "Defs.h"

QString FindValueByKey(const STRING_PAIR pairs[], const QString &key)
{
    int i = 0;
    while (strlen(pairs[i].key) != 0)
    {
        if (key == pairs[i].key)
        {
            return pairs[i].text;
        }
        i++;
    }
    return "";
}

QString GetLoudspeakerName(const LOUDSPEAKERNAME Ls)
{
    if (Ls > LS_FrontWideRight)
        return "";
    return LOUDSPEAKERNAME_LIST[Ls];
}

QString GetEQFrequencyIdentifier(const EQ_FREQUENCY freq)
{
    if (freq > EQ_Wide_Trim)
        return "";
    return EQ_FREQUENCY_LIST[freq];
}

const STRING_PAIR AST1[] = {
    {"00", "ANALOG"},
    {"01", "ANALOG"},
    {"02", "ANALOG"},
    {"03", "PCM"},
    {"04", "PCM"},
    {"05", "DOLBY DIGITAL"},
    {"06", "DTS"},
    {"07", "DTS-ES Matrix"},
    {"08", "DTS-ES Discrete"},
    {"09", "DTS 96/24"},
    {"10", "DTS 96/24 ES Matrix"},
    {"11", "DTS 96/24 ES Discrete"},
    {"12", "MPEG-2 AAC"},
    {"13", "WMA9 Pro"},
    {"14", "DSD->PCM"},
    {"15", "HDMI THROUGH"},
    {"16", "DD PLUS"},
    {"17", "DOLBY TrueHD"},
    {"18", "DTS EXPRESS"},
    {"19", "DTS-HD MA"},
    {"20", "DTS-HD HiRes"},
    {"21", "DTS-HD HiRes"},
    {"22", "DTS-HD HiRes"},
    {"23", "DTS-HD HiRes"},
    {"24", "DTS-HD HiRes"},
    {"25", "DTS-HD HiRes"},
    {"26", "DTS-HD HiRes"},
    {"27", "DTS-HD MA"},
    {"28", "DSD"},// (HDMI or File via DSD DIRECT route)"},
    {"64", "MP3"},
    {"65", "WAV"},
    {"66", "WMA"},
    {"67", "MPEG4-AAC"},
    {"68", "FLAC"},
    {"69", "ALAC"},//(Apple Lossless)"},
    {"70", "AIFF"},
    {"71", "DSD"},// (USB-DAC)"},

};

const STRING_PAIR AST3[] = {
    {"00", "32kHz"},
    {"01", "44.1kHz"},
    {"02", "48kHz"},
    {"03", "88.2kHz"},
    {"04", "96kHz"},
    {"05", "176.4kHz"},
    {"06", "192kHz"},
    {"07", "---"},
    {"32", "2.8MHz"},
    {"33", "5.6MHz"},
};


const STRING_PAIR LISTENING_MODE[]  = {
    {"0001", "STEREO (cyclic)"},
    {"0005", "AUTO SURR/STREAM DIRECT (cyclic)"},
    {"0006", "AUTO SURROUND"},
    {"0151", "ALC"},
    {"0007", "DIRECT"},
    {"0008", "PURE DIRECT"},
    {"0009", "STEREO (direct set)"},
    {"0003", "Front Stage Surround Advance Focus"},
    {"0004", "Front Stage Surround Advance Wide"},
    {"0153", "RETRIEVER AIR"},
    {"0010", "STANDARD"},
    {"0011", "(2ch source)"},
    {"0013", "PRO LOGIC2 MOVIE"},
    {"0018", "PRO LOGIC2x MOVIE"},
    {"0014", "PRO LOGIC2 MUSIC"},
    {"0019", "PRO LOGIC2x MUSIC"},
    {"0015", "PRO LOGIC2 GAME"},
    {"0020", "PRO LOGIC2x GAME"},
    {"0031", "PRO LOGIC2z Height"},
    {"0032", "WIDE SURROUND MOVIE"},
    {"0033", "WIDE SURROUND MUSIC"},
    {"0012", "PRO LOGIC"},
    {"0016", "Neo:6 CINEMA"},
    {"0017", "Neo:6 MUSIC"},
    {"0028", "XM HD SURROUND"},
    {"0029", "NEURAL SURROUND"},
    {"0021", "(Multi ch source)"},
    {"0022", "(Multi ch source)+DOLBY EX"},
    {"0023", "(Multi ch source)+PRO LOGIC2x MOVIE"},
    {"0024", "(Multi ch source)+PRO LOGIC2x MUSIC"},
    {"0034", "(Multi-ch Source)+PRO LOGIC2z HEIGHT"},
    {"0035", "(Multi-ch Source)+WIDE SURROUND MOVIE"},
    {"0036", "(Multi-ch Source)+WIDE SURROUND MUSIC"},
    {"0025", "DTS-ES Neo:6"},
    {"0026", "DTS-ES matrix"},
    {"0027", "DTS-ES discrete"},
    {"0030", "DTS-ES 8ch discrete"},
    {"0100", "ADVANCED SURROUND (cyclic)"},
    {"0101", "ACTION"},
    {"0103", "DRAMA"},
    {"0102", "SCI-FI"},
    {"0105", "MONO FILM"},
    {"0104", "ENTERTAINMENT SHOW"},
    {"0106", "EXPANDED THEATER"},
    {"0116", "TV SURROUND"},
    {"0118", "ADVANCED GAME"},
    {"0117", "SPORTS"},
    {"0107", "CLASSICAL"},
    {"0110", "ROCK/POP"},
    {"0109", "UNPLUGGED"},
    {"0112", "EXTENDED STEREO"},
    {"0113", "PHONES SURROUND"},
    {"0050", "THX (cyclic)"},
    {"0051", "PROLOGIC + THX CINEMA"},
    {"0052", "PL2 MOVIE + THX CINEMA"},
    {"0053", "Neo:6 CINEMA + THX CINEMA"},
    {"0054", "PL2x MOVIE + THX CINEMA"},
    {"0092", "PL2z HEIGHT + THX CINEMA"},
    {"0055", "THX SELECT2 GAMES"},
    {"0068", "THX CINEMA (for 2ch)"},
    {"0069", "THX MUSIC (for 2ch)"},
    {"0070", "THX GAMES (for 2ch)"},
    {"0071", "PL2 MUSIC + THX MUSIC"},
    {"0072", "PL2x MUSIC + THX MUSIC"},
    {"0093", "PL2z HEIGHT + THX MUSIC"},
    {"0073", "Neo:6 MUSIC + THX MUSIC"},
    {"0074", "PL2 GAME + THX GAMES"},
    {"0075", "PL2x GAME + THX GAMES"},
    {"0094", "PL2z HEIGHT + THX GAMES"},
    {"0076", "THX ULTRA2 GAMES"},
    {"0077", "PROLOGIC + THX MUSIC"},
    {"0078", "PROLOGIC + THX GAMES"},
    {"0056", "THX CINEMA (for multi ch)"},
    {"0057", "THX SURROUND EX (for multi ch)"},
    {"0058", "PL2x MOVIE + THX CINEMA (for multi ch)"},
    {"0095", "PL2z HEIGHT + THX CINEMA (for multi ch)"},
    {"0059", "ES Neo:6 + THX CINEMA (for multi ch)"},
    {"0060", "ES MATRIX + THX CINEMA (for multi ch)"},
    {"0061", "ES DISCRETE + THX CINEMA (for multi ch)"},
    {"0067", "ES 8ch DISCRETE + THX CINEMA (for multi ch)"},
    {"0062", "THX SELECT2 CINEMA (for multi ch)"},
    {"0063", "THX SELECT2 MUSIC (for multi ch)"},
    {"0064", "THX SELECT2 GAMES (for multi ch)"},
    {"0065", "THX ULTRA2 CINEMA (for multi ch)"},
    {"0066", "THX ULTRA2 MUSIC (for multi ch)"},
    {"0079", "THX ULTRA2 GAMES (for multi ch)"},
    {"0080", "THX MUSIC (for multi ch)"},
    {"0081", "THX GAMES (for multi ch)"},
    {"0082", "PL2x MUSIC + THX MUSIC (for multi ch)"},
    {"0096", "PL2z HEIGHT + THX MUSIC (for multi ch)"},
    {"0083", "EX + THX GAMES (for multi ch)"},
    {"0097", "PL2z HEIGHT + THX GAMES (for multi ch)"},
    {"0084", "Neo:6 + THX MUSIC (for multi ch)"},
    {"0085", "Neo:6 + THX GAMES (for multi ch)"},
    {"0086", "ES MATRIX + THX MUSIC (for multi ch)"},
    {"0087", "ES MATRIX + THX GAMES (for multi ch)"},
    {"0088", "ES DISCRETE + THX MUSIC (for multi ch)"},
    {"0089", "ES DISCRETE + THX GAMES (for multi ch)"},
    {"0090", "ES 8CH DISCRETE + THX MUSIC (for multi ch)"},
    {"0091", "ES 8CH DISCRETE + THX GAMES (for multi ch)"},
    {"0152", "OPTIMUM SURROUND"},
    {"", ""},
};

const STRING_PAIR LISTENING_MODE_ONKYO[] = {
    {"00", "STEREO"},
    {"01", "DIRECT"},
    {"03", "FILM/Game-RPG"},
    {"05", "ACTION/Game-Action"},
    {"06", "MUSICAL/Game-Rock"},
    {"08", "CLASSICAL"}, // ORCHESTRA
    {"09", "UNPLUGGED"},
    {"0A", "ENT. SHOW"}, // "STUDIO-MIX"
    {"0B", "DRAMA"}, // TV LOGIC
    {"0C", "EXT. STEREO"}, // "ALL CH STEREO"
    {"0D", "F.S. SURROUND"}, // "THEATER-DIMENSIONAL"
    {"0E", "ENHANCED 7/ENHANCE/Game-Sports"},
    {"0F", "MONO"},
    {"11", "PURE AUDIO"},
    {"13", "EXT. MONO"}, //FULL MONO
    {"1F", "Multi Zone Music"},
    {"40", "Straight Decode"},
    {"80", "PLII Movie/Dolby Atmos/Dolby Surround"},
    {"81", "PLII Music"},
    {"82", "Neo:6 Cinema/DTS:X/Neural:X"},
    {"83", "Neo:6 Music"},
    {"86", "PLII Game"},
    {"FF", "Auto Surround"},
    {"", ""},
};

const STRING_PAIR PLAYING_LISTENING_MODE[]  = {
    {"0001", "STEREO"},
    {"0002", "F.S.SURR FOCUS"},
    {"0003", "F.S.SURR WIDE"},
    {"0004", "RETRIEVER AIR"},
    {"0101", "[)(]PLIIx MOVIE"},
    {"0102", "[)(]PLII MOVIE"},
    {"0103", "[)(]PLIIx MUSIC"},
    {"0104", "[)(]PLII MUSIC"},
    {"0105", "[)(]PLIIx GAME"},
    {"0106", "[)(]PLII GAME"},
    {"0107", "[)(]PROLOGIC"},
    {"0108", "Neo:6 CINEMA"},
    {"0109", "Neo:6 MUSIC"},
    {"010a", "XM HD Surround"},
    {"010b", "NEURAL SURR"},
    {"010c", "2ch Straight Decode"},
    {"010d", "[)(]PLIIz HEIGHT"},
    {"010e", "WIDE SURR MOVIE"},
    {"010f", "WIDE SURR MUSIC"},
    {"1101", "[)(]PLIIx MOVIE"},
    {"1102", "[)(]PLIIx MUSIC"},
    {"1103", "[)(]DIGITAL EX"},
    {"1104", "DTS +Neo:6 / DTS-HD +Neo:6"},
    {"1105", "ES MATRIX"},
    {"1106", "ES DISCRETE"},
    {"1107", "DTS-ES 7.1"},
    {"1108", "multi ch Straight Decode"},
    {"1109", "[)(]PLIIz HEIGHT"},
    {"110a", "WIDE SURR MOVIE"},
    {"110b", "WIDE SURR MUSIC"},
    {"0201", "ACTION"},
    {"0202", "DRAMA"},
    {"0203", "SCI-FI"},
    {"0204", "MONOFILM"},
    {"0205", "ENT.SHOW"},
    {"0206", "EXPANDED"},
    {"0207", "TV SURROUND"},
    {"0208", "ADVANCEDGAME"},
    {"0209", "SPORTS"},
    {"020a", "CLASSICAL"},
    {"020b", "ROCK/POP"},
    {"020c", "UNPLUGGED"},
    {"020d", "EXT.STEREO"},
    {"020e", "PHONES SURR."},
    {"0301", "[)(]PLIIx MOVIE +THX"},
    {"0302", "[)(]PLII MOVIE +THX"},
    {"0303", "[)(]PL +THX CINEMA"},
    {"0304", "Neo:6 CINEMA +THX"},
    {"0305", "THX CINEMA"},
    {"0306", "[)(]PLIIx MUSIC +THX"},
    {"0307", "[)(]PLII MUSIC +THX"},
    {"0308", "[)(]PL +THX MUSIC"},
    {"0309", "Neo:6 MUSIC +THX"},
    {"030a", "THX MUSIC"},
    {"030b", "[)(]PLIIx GAME +THX"},
    {"030c", "[)(]PLII GAME +THX"},
    {"030d", "[)(]PL +THX GAMES"},
    {"030e", "THX ULTRA2 GAMES"},
    {"030f", "THX SELECT2 GAMES"},
    {"0310", "THX GAMES"},
    {"0311", "[)(]PLIIz +THX CINEMA"},
    {"0312", "[)(]PLIIz +THX MUSIC"},
    {"0313", "[)(]PLIIz +THX GAMES"},
    {"1301", "THX Surr EX"},
    {"1302", "Neo:6 +THX CINEMA"},
    {"1303", "ES MTRX +THX CINEMA"},
    {"1304", "ES DISC +THX CINEMA"},
    {"1305", "ES7.1 +THX CINEMA"},
    {"1306", "[)(]PLIIx MOVIE +THX"},
    {"1307", "THX ULTRA2 CINEMA"},
    {"1308", "THX SELECT2 CINEMA"},
    {"1309", "THX CINEMA"},
    {"130a", "Neo:6 +THX MUSIC"},
    {"130b", "ES MTRX +THX MUSIC"},
    {"130c", "ES DISC +THX MUSIC"},
    {"130d", "ES7.1 +THX MUSIC"},
    {"130e", "[)(]PLIIx MUSIC +THX"},
    {"130f", "THX ULTRA2 MUSIC"},
    {"1310", "THX SELECT2 MUSIC"},
    {"1311", "THX MUSIC"},
    {"1312", "Neo:6 +THX GAMES"},
    {"1313", "ES MTRX +THX GAMES"},
    {"1314", "ES DISC +THX GAMES"},
    {"1315", "ES7.1 +THX GAMES"},
    {"1316", "[)(]EX +THX GAMES"},
    {"1317", "THX ULTRA2 GAMES"},
    {"1318", "THX SELECT2 GAMES"},
    {"1319", "THX GAMES"},
    {"131a", "[)(]PLIIz +THX CINEMA"},
    {"131b", "[)(]PLIIz +THX MUSIC"},
    {"131c", "[)(]PLIIz +THX GAMES"},
    {"0401", "STEREO"},
    {"0402", "[)(]PLII MOVIE"},
    {"0403", "[)(]PLIIx MOVIE"},
    {"0404", "Neo:6 CINEMA"},
    {"0405", "AUTO SURROUND Straight Decode"},
    {"0406", "[)(]DIGITAL EX"},
    {"0407", "[)(]PLIIx MOVIE"},
    {"0408", "DTS +Neo:6"},
    {"0409", "ES MATRIX"},
    {"040a", "ES DISCRETE"},
    {"040b", "DTS-ES 7.1"},
    {"040c", "XM HD Surround"},
    {"040d", "NEURALSURR"},
    {"040e", "RETRIEVER AIR"},
    {"0501", "ALC STEREO"},
    {"0502", "[)(]PLII MOVIE"},
    {"0503", "[)(]PLIIx MOVIE"},
    {"0504", "Neo:6 CINEMA"},
    {"0505", "ALC Multi Channel"}, //"ALC Straight Decode"
    {"0506", "[)(]DIGITAL EX"},
    {"0507", "[)(]PLIIx MOVIE"},
    {"0508", "DTS +Neo:6"},
    {"0509", "ES MATRIX"},
    {"050a", "ES DISCRETE"},
    {"050b", "DTS-ES 7.1"},
    {"050c", "XM HD Surround"},
    {"050d", "NEURAL SURR"},
    {"050e", "RETRIEVER AIR"},
    {"0601", "STEREO"},
    {"0602", "[)(]PLII MOVIE"},
    {"0603", "[)(]PLIIx MOVIE"},
    {"0604", "Neo:6 CINEMA"},
    {"0605", "STREAM DIRECT NORMAL Straight Decode"},
    {"0606", "[)(]DIGITAL EX"},
    {"0607", "[)(]PLIIx MOVIE"},
    {"0608", "(nothing)"},
    {"0609", "ES MATRIX"},
    {"060a", "ES DISCRETE"},
    {"060b", "DTS-ES 7.1"},
    {"0701", "STREAM DIRECT PURE 2ch"},
    {"0702", "[)(]PLII MOVIE"},
    {"0703", "[)(]PLIIx MOVIE"},
    {"0704", "Neo:6 CINEMA"},
    {"0705", "PURE DIRECT Straight Decode"},
    {"0706", "[)(]DIGITAL EX"},
    {"0707", "[)(]PLIIx MOVIE"},
    {"0708", "(nothing)"},
    {"0709", "ES MATRIX"},
    {"070a", "ES DISCRETE"},
    {"070b", "DTS-ES 7.1"},
    {"0881", "OPTIMUM"},
    {"0e01", "HDMI THROUGH"},
    {"0f01", "MULTI CH IN"},
    {"", ""},
};

const char* LOUDSPEAKERNAME_LIST[] = {
    "L__", //Front Left
    "R__", //Front Right
    "C__", //Center
    "SL_", //Surround Left
    "SR_", //Surround Right
    "SBL", //Surround Back Left
    "SBR", //Surround Back Right
    "LH_", //Front Height Left
    "RH_", //Front Height Right
    "LW_", //Front Wide Left
    "RW_", //Front Wide Right
};

const char* EQ_FREQUENCY_LIST[] = {
    "00", //63Hz
    "01", //125Hz
    "02", //250Hz
    "03", //500Hz
    "04", //1kHz
    "05", //2kHz
    "06", //4kHz
    "07", //8kHz
    "08", //16kHz
    "09", //EQ Wide Trim
};
