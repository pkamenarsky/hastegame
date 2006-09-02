#ifndef ION_VIDEO_PROGRAMARRAY_HH_INCLUDED
#define ION_VIDEO_PROGRAMARRAY_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/basememobj.hh"
#include "../base/datablock.hh"
#include "../base/iontypes.hh"
#include "../base/localstreamable.hh"
#include "../base/string.hh"

namespace ion {
namespace video {

	class Program;
	class Videodevice;

	class ION_API Programarray:public base::BaseMemObj
	{
	public:
		enum Type {
			Vertexprograms,
			Fragmentprograms
		};

		static Programarray* loadFromXML(base::Streamable& streamable,Videodevice &rVideodevice);
		static Programarray* loadFromXML(base::LocalStreamable& localstreamable,Videodevice &rVideodevice);
		static Programarray* loadFromXML(base::Datablock& datablock,Videodevice &rVideodevice,const bool addNullbyte=true);

		Programarray(Videodevice &rVideodevice,const Type programstype);
		Programarray(const Programarray& src);
		~Programarray();

		Type programstype() const;
		Videodevice& videodevice() const;

		void clone(const Programarray& src);
		Programarray& operator =(const Programarray& src);

		ion_uint32 numPrograms() const;
		Program* program(const ion_uint32 index) const;
		bool addProgram(const base::String& identifier,
			const base::String& entrypoint,const base::String& program,const base::String& format);

		video::Program* getFirstValidProgram();

		bool isValid() const;

		struct Internaldata;
		const Internaldata& internaldata() const;
	protected:
		Videodevice &m_rVideodevice;
		Internaldata *m_pInternaldata;
		Type m_Programstype;
	};

}
}

#endif
