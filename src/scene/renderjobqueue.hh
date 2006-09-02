#ifndef ION_SCENE_RENDERJOBQUEUE_HH_INCLUDED
#define ION_SCENE_RENDERJOBQUEUE_HH_INCLUDED

#include <vector>
#include "../base/basememobj.hh"
#include "../video/indexstream.hh"
#include "../video/vertexstream.hh"

namespace ion {
namespace scene {

	class Renderable;
	class Renderjob;
	class Camera;

	class Renderjobqueue:public base::BaseMemObj
	{
	public:
		ION_API Renderjobqueue();
		ION_API ~Renderjobqueue();

		ION_API void clear();
		ION_API void deleteJobs();
		ION_API bool isValid() const;
		ION_API void addJob(Renderjob& rRenderjob);
		ION_API ion_uint32 numJobs() const;
		ION_API Renderjob* job(const ion_uint32 jobnr);

		ION_API void appendQueue(const Renderjobqueue& queue);

		ION_API void sortJobs(const math::Vector3f& viewerPosition);

		std::vector< Renderjob * >& jobs() const;
		std::vector< Renderjob * >& shadedOpaqueJobs();
		std::vector< Renderjob * >& unshadedOpaqueJobs();
		std::vector< Renderjob * >& blendedJobs();

	protected:
		static bool sortShadedOpaqueJobs(const Renderjob* pJob1,const Renderjob* pJob2);
		static bool sortBlendedJobs(const Renderjob* pJob1,const Renderjob* pJob2);

		struct Joblist;
		Joblist *m_pJoblist;
	};

}
}

#endif
