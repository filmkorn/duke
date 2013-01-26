/*
 * AsciiFont.cpp
 *
 *  Created on: Jan 12, 2013
 *      Author: Guillaume Chatelet
 */

#include <duke/imageio/DukeIO.h>
#include <duke/gl/GL.h>
#include <bitset>
#include <cstring>

static const uint64_t raster_data[] = { 0x81c08367e7e00, 0x18fcfc3cf0ff00ff, 0x83e1c7fff8100, 0xdb848442c0c33cff, 0x181c1c3e7fdba500, 0x3cfcfc42a09966e7, 0x3c3e7f7f7fff8100,
		0xe7840442bebd42c3, 0x3c7f7f3e3ec3bd00, 0xe7c4043c21bd42c3, 0x183e6b1c1ce79900, 0x3ce60618219966e7, 0x8080808ff8100, 0xdb67077e21c33cff, 0x1c1c00007e7e00,
		0x180303181eff00ff, 0x18007cfe24184001, 0x808, 0x3c00c649243c7007, 0x7f0824000c18081c, 0x7e001c49247e7c1f, 0x7f1c66020630083e, 0x1800224e24187f7f, 0x3e3eff027f7f2a2a,
		0x7e7e224824187c1f, 0x1c7f660206303e08, 0x3c7e1c48007e7007, 0x87f247e0c181c08, 0x187e3348243c4001, 0x808, 0xff001e0000180000, 0x0, 0x80c001824240800, 0x408,
		0x812467c24241c00, 0x4000000008220804, 0x40c26027e241c00, 0x20000000081c1002, 0x6a103c24000800, 0x10007e003e7f1002, 0x1108407e000800, 0x8000000081c1002, 0x11643e24000000,
		0x408000808220804, 0x6e621824000800, 0x208000800000408, 0x0, 0x400000000, 0x7e387e103c3c083c, 0x3c08001000003c3c, 0x4204021842420c42, 0x4210000808084242,
		0x20023e1440400a62, 0x40207e0408084242, 0x103e401238300852, 0x2040000200007c3c, 0x842407f400c084a, 0x1020000400004042, 0x842421042420846, 0x107e0808082042,
		0x83c3c383c7e3e3c, 0x1008001008081c3c, 0x0, 0x4000000, 0x387e7e1e383e183c, 0x1846c60e46701c42, 0x4444442444442442, 0x244aaa0424200842, 0x21414440244427a,
		0x4252920414200842, 0x21c1c44023c424a, 0x426282040c20087e, 0x7214144402447e7a, 0x4242820414220842, 0x4404442444444202, 0x2442824424220842, 0x780e7e1e383e423c,
		0x1842827ec61c1c42, 0x0, 0x0, 0x828242fe3c3e3c3e, 0x81e011efe8282, 0x8282429242444244, 0x14100202424444, 0x8282421002444244, 0x22100402202828, 0x928242103c3c423c,
		0x41100802101010, 0x9244421040145204, 0x101002081028, 0x9228421042243c04, 0x102002841044, 0x6c103c383c4ec00e, 0x1e401efe3882, 0x0, 0xff00000000000000, 0x30006000060008,
		0xc06400806, 0x48004000040008, 0x804000004, 0xbc083c403c043c10, 0x3c3a6e0824600c34, 0x421c425c42744000, 0x424692081440084c, 0x42087e62028c7c00, 0x424292080c400844,
		0x7c080262428c4200, 0x4242920814420844, 0x401c3cdc3c74fc00, 0x3c42921c64421c46, 0x3e00000000000000, 0x3c0000, 0x800000000, 0x4c0c0830000000, 0x800000000,
		0x1032100808000000, 0x8282423e7c366c36, 0x28001008083e4222, 0x92824208024c324c, 0x4400600006104214, 0x924442083c44324c, 0x8200100808084208, 0x9228624840042c34,
		0x8200100808047c14, 0x6c105c303e0e2004, 0xfe000c08303e4022, 0x700e, 0x3e00, 0x80c423c30003c, 0x18420c3e240c423c, 0x42002242, 0x1818004100000042, 0x3c1c1c1c1c3c0002,
		0x24180c183c3c3c, 0x220202020422242, 0x3c42100810424242, 0x23c3c3c3c7e223c, 0x427e1008107e7e7e, 0x3c22222222022230, 0x7e42100810020202, 0x607c7c7c7c3c7c40,
		0x4242381c383c3c3c, 0x380000000000003c, 0x0, 0x418040018f80030, 0x381f221810424200, 0x824084224240000, 0x4832142410001842, 0x22cc3e, 0x81e3e047c422400, 0x42423c3c3cfe3004,
		0x3e22080e02424242, 0x424242424222fc1c, 0x8f23e0402424242, 0x4242424242222204, 0x82208427c42247c, 0x3c3c3c3c3ce2dc3e, 0x9a2083e103c1840, 0x0, 0x66700001000003c,
		0x1c3c2c4c00203030, 0x4242000008, 0x2222323220100000, 0x1248082323000000, 0x222200001000181c, 0x2424005212000008, 0x1c7c463e423c1020, 0x4812086f6f7e7e04, 0x4a424242103c,
		0x2424085494400202, 0x3e7e524242421022, 0x124808fac2400242, 0x62423c3c387c, 0x8413100003c, 0x0, 0x40f8000000, 0x28080808dbaa44, 0x8282800282800, 0x28080808ee5511,
		0x8282800282800, 0x280f0808dbaa44, 0xf282f3f282f0f, 0x28080808775511, 0x8282020282008, 0x3f2f0f0f08dbaa44, 0xf0f3f3f2f282f0f, 0x2828080808ee5511, 0x800000028282808,
		0x2828080808dbaa44, 0x800000028282808, 0x2828080808775511, 0x800000028282808, 0x2808080008000808, 0x828002800280028, 0x2808080008000808, 0x828002800280028,
		0x28f8080008000808, 0xffefffe8ffeff8e8, 0x2808080008000808, 0x800000808, 0xe8f8fffff8fffff8, 0xffefffe8efffe8f8, 0x2808080008080000, 0x28002828002800, 0x2808080008080000,
		0x28002828002800, 0x2808080008080000, 0x28002828002800, 0x2800000828000028, 0xfff00f00ff000808, 0x2800000828000028, 0xfff00f00ff000808, 0x2800f8f82800ff28,
		0xfff00f00ff0008ff, 0x2800080828000028, 0xfff00f00ff000808, 0xfff8f8f8f8ffffff, 0xf00ffffff80fff, 0x2828080000280800, 0xf00fffff080008, 0x2828080000280800,
		0xf00fffff080008, 0x2828080000280800, 0xf00fffff080008, 0x7e00000000, 0x3c3040003818183e, 0xcc220042fc7e3c00, 0x4208204604242408, 0x32227c042a42428c, 0x42043ca91842421c,
		0x1022121828023e52, 0x423c52913c427e22, 0x105e120428024222, 0x42044aa942244222, 0x1002124228023e52, 0x42083c464224241c, 0x10020c7e2802028c, 0x423002003c661808,
		0x1000000000200, 0x1000000003e, 0x18083010080800, 0xc1ef000000c, 0x4c1808480810087e, 0x122210000012, 0x3200084804203e00, 0x3c082210000012, 0x7e08080810087e,
		0x3c04221000180c, 0x4c00080810080800, 0x3c1e2210181800, 0x321809080000007e, 0x3c000013000000, 0x1809087e7e3e00, 0x14000000, 0x60800000000, 0x18000000 };

class AsciiFontImageReader: public IImageReader {
public:
	AsciiFontImageReader(const IIODescriptor *pDesc) :
			IImageReader(pDesc) {
		m_Description.height = 128;
		m_Description.width = 128;
		m_Description.glPackFormat = GL_RGBA8;
		m_Description.dataSize = m_Description.height * m_Description.width * sizeof(uint32_t);

	}

	virtual void readImageDataTo(void* pData) {
		const static unsigned char pOn[] = { 0xFF, 0xFF, 0xFF, 0xFF };
		const static unsigned char pOff[] = { 0x00, 0x00, 0x00, 0x00 };
		using namespace std;
		auto pCharData = reinterpret_cast<unsigned char*>(pData);
		for (size_t index = 0; index < 256; ++index) {
			const std::bitset<64> bitset(raster_data[index]);
			for (size_t i = 0; i < bitset.size(); ++i) {
				memcpy(pCharData, bitset[i] ? pOn : pOff, 4);
				pCharData += sizeof(int);
			}
		}
	}
};

class AsciiFontDescriptor: public IIODescriptor {
	virtual bool supports(Capability capability) const {
		return false;
	}
	virtual const std::vector<std::string>& getSupportedExtensions() const {
		static std::vector<std::string> extensions = { "duke_ascii_font" };
		return extensions;
	}
	virtual const char* getName() const {
		return "Basic font provider";
	}
	virtual IImageReader* getReaderFromFile(const char *filename) const {
		return new AsciiFontImageReader(this);
	}
};

namespace {
bool registrar = IODescriptors::instance().registerDescriptor(new AsciiFontDescriptor());
}  // namespace
