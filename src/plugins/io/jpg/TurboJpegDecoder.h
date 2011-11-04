#ifndef TURBOJPEGDECODER_H_
#define TURBOJPEGDECODER_H_

#include <dukeio/ImageDescription.h>
#include <dukeplugin/IBoostPlugin.h>
#include <dukeplugin/suite/property/PropertySuiteImpl.h>

#include <turbojpeg.h>

class TurboJpegDecoder : public IBoostPlugin
{
    openfx::plugin::PropertySuiteImpl m_PropertySuite;

public:
	TurboJpegDecoder();
	virtual ~TurboJpegDecoder();

    OfxStatus noOp();
	OfxStatus createInstance(const void* handle, OfxPropertySetHandle in, OfxPropertySetHandle out);
	OfxStatus destroyInstance(const void* handle, OfxPropertySetHandle in, OfxPropertySetHandle out);
	OfxStatus describe(const void* handle, OfxPropertySetHandle in, OfxPropertySetHandle out);
	OfxStatus readHeader(const void* handle, OfxPropertySetHandle in, OfxPropertySetHandle out);
	OfxStatus decodeImage(const void* handle, OfxPropertySetHandle in, OfxPropertySetHandle out);
};

#endif /* TURBOJPEGDECODER_H_ */

