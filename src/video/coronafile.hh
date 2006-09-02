#ifndef ION_VIDEO_CORONAFILE_HH_INCLUDED
#define ION_VIDEO_CORONAFILE_HH_INCLUDED

namespace ion {
namespace video {

/////// THIS CLASS IS FOR INTERNAL USE ONLY

	class CoronaFile:public corona::DLLImplementation<corona::File>
	{
	public:
		CoronaFile(base::LocalStreamable& rStreamable):m_rStreamable(rStreamable) {}
		~CoronaFile() {}
	
		int COR_CALL read(void* buffer, int size) { return m_rStreamable.read(buffer,size); }
		int COR_CALL write(const void* buffer, int size) { return m_rStreamable.write(buffer,size); }
		bool COR_CALL seek(int position, corona::File::SeekMode mode) {
			base::LocalStreamable::Seekmode seekmode=base::LocalStreamable::SeekAbsolute;
			switch (mode) {
				case corona::File::BEGIN:seekmode=base::LocalStreamable::SeekAbsolute; break;
				case corona::File::CURRENT:seekmode=base::LocalStreamable::SeekRelative; break;
				case corona::File::END:seekmode=base::LocalStreamable::SeekFromEnd; break;
			}
			m_rStreamable.seek(position,seekmode);
			return true;
		}
		int COR_CALL tell() { return m_rStreamable.tell(); }
		
		//void COR_CALL destroy() { delete this; }
	protected:
		base::LocalStreamable& m_rStreamable;
	};

}
}

#endif
