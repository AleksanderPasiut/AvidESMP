#include "track_data.h"

void TRACK_DATA::BASS_Load()
{
	handle.hstream = BASS_StreamCreateFile(false,
										   url_file.c_str(),
										   0,
										   0,
										   BASS_STREAM_PRESCAN);
	BASS_ChannelSetSync(handle.hstream, BASS_SYNC_END, 0, reinterpret_cast<SYNCPROC*>(TrackEnd), 0);

	if (!handle.hstream)
		throw 0;

	player_type = TPT_BASS;

	BASS_RetrieveFileType();
	BASS_RetrieveDuration();
	BASS_RetrieveBitrate();
}
void TRACK_DATA::BASS_RetrieveFileType() noexcept
{
	BASS_CHANNELINFO info;
	if (BASS_ChannelGetInfo(handle.hstream, &info))
		switch(info.ctype)
		{
			case BASS_CTYPE_STREAM_OGG: type = TT_OGG; break;
			case BASS_CTYPE_STREAM_MP1: type = TT_MP1; break;
			case BASS_CTYPE_STREAM_MP2: type = TT_MP2; break;
			case BASS_CTYPE_STREAM_MP3: type = TT_MP3; break;
			case BASS_CTYPE_STREAM_AIFF: type = TT_AIFF; break;
			case BASS_CTYPE_STREAM_CA: type = TT_CA; break;
			case BASS_CTYPE_STREAM_MF: type = TT_WMA; break;
			case BASS_CTYPE_STREAM_WAV_PCM: type = TT_WAV_Int; break;
			case BASS_CTYPE_STREAM_WAV_FLOAT: type = TT_WAV_Float; break;
		}
}
void TRACK_DATA::BASS_RetrieveDuration() noexcept
{
	__int64 qword_length = BASS_ChannelGetLength(handle.hstream, BASS_POS_BYTE);

	if (qword_length == -1)
		return;

	duration = static_cast<double>(BASS_ChannelBytes2Seconds(handle.hstream, qword_length));
}
void TRACK_DATA::BASS_RetrieveBitrate() noexcept
{
	__int64 qword = BASS_StreamGetFilePosition(handle.hstream, BASS_FILEPOS_END);

	if (qword == -1)
		return;

	__int64 qword_length = BASS_ChannelGetLength(handle.hstream, BASS_POS_BYTE);

	if (qword_length == -1)
		return;

	float tmp_duration = static_cast<float>(BASS_ChannelBytes2Seconds(handle.hstream, qword_length));

	if (tmp_duration < 0.0f)
		return;

	bitrate = static_cast<double>(qword)/(tmp_duration*125.0);
}
