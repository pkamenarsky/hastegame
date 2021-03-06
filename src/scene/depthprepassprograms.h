// Autogenerated from prepassarrays.xml
// DO NOT EDIT THIS FILE! Edit prepassarrays.xml instead

#ifndef AUTOGENERATED_DEPTHPREPASSPROGRAMS_HPP_5ZY4HSUIYKHTPFPN7Q30ROE94YXWQNBI_INCLUDED
#define AUTOGENERATED_DEPTHPREPASSPROGRAMS_HPP_5ZY4HSUIYKHTPFPN7Q30ROE94YXWQNBI_INCLUDED

namespace ion {
namespace video {
	class Videodevice;
	class Program;
	class Programarray;
}
}

class DepthPrepassPrograms
{
public:

	DepthPrepassPrograms(ion::video::Videodevice &rDevice);
	~DepthPrepassPrograms();

	ion::video::Programarray& programarray_prepassvp();
	const ion::video::Programarray& programarray_prepassvp() const;
	ion::video::Program* firstWorkingProgram_prepassvp();
	const ion::video::Program* firstWorkingProgram_prepassvp() const;

	ion::video::Programarray& programarray_prepassfp();
	const ion::video::Programarray& programarray_prepassfp() const;
	ion::video::Program* firstWorkingProgram_prepassfp();
	const ion::video::Program* firstWorkingProgram_prepassfp() const;

protected:
	DepthPrepassPrograms(const DepthPrepassPrograms& src);
	DepthPrepassPrograms& operator =(const DepthPrepassPrograms& src);

	ion::video::Programarray* m_pProgramarray_prepassvp;
	ion::video::Program* m_pFirstWorkingProgram_prepassvp;
	ion::video::Programarray* m_pProgramarray_prepassfp;
	ion::video::Program* m_pFirstWorkingProgram_prepassfp;
};

#endif
