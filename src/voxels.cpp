//
// Created by zyuiop on 18/05/19.
//

#include "voxels.h"
#include "glmath.h"

#include <thread>

Voxel::Voxel(int8_t x, int8_t y, int8_t z, int8_t color)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->color = color;
}

const long default_palette[256] = {
		0x00000000L, 0xffffffffL, 0xffccffffL, 0xff99ffffL, 0xff66ffffL, 0xff33ffffL, 0xff00ffffL, 0xffffccffL, 0xffccccffL, 0xff99ccffL, 0xff66ccffL, 0xff33ccffL, 0xff00ccffL, 0xffff99ffL, 0xffcc99ffL, 0xff9999ffL,
		0xff6699ffL, 0xff3399ffL, 0xff0099ffL, 0xffff66ffL, 0xffcc66ffL, 0xff9966ffL, 0xff6666ffL, 0xff3366ffL, 0xff0066ffL, 0xffff33ffL, 0xffcc33ffL, 0xff9933ffL, 0xff6633ffL, 0xff3333ffL, 0xff0033ffL, 0xffff00ffL,
		0xffcc00ffL, 0xff9900ffL, 0xff6600ffL, 0xff3300ffL, 0xff0000ffL, 0xffffffccL, 0xffccffccL, 0xff99ffccL, 0xff66ffccL, 0xff33ffccL, 0xff00ffccL, 0xffffccccL, 0xffccccccL, 0xff99ccccL, 0xff66ccccL, 0xff33ccccL,
		0xff00ccccL, 0xffff99ccL, 0xffcc99ccL, 0xff9999ccL, 0xff6699ccL, 0xff3399ccL, 0xff0099ccL, 0xffff66ccL, 0xffcc66ccL, 0xff9966ccL, 0xff6666ccL, 0xff3366ccL, 0xff0066ccL, 0xffff33ccL, 0xffcc33ccL, 0xff9933ccL,
		0xff6633ccL, 0xff3333ccL, 0xff0033ccL, 0xffff00ccL, 0xffcc00ccL, 0xff9900ccL, 0xff6600ccL, 0xff3300ccL, 0xff0000ccL, 0xffffff99L, 0xffccff99L, 0xff99ff99L, 0xff66ff99L, 0xff33ff99L, 0xff00ff99L, 0xffffcc99L,
		0xffcccc99L, 0xff99cc99L, 0xff66cc99L, 0xff33cc99L, 0xff00cc99L, 0xffff9999L, 0xffcc9999L, 0xff999999L, 0xff669999L, 0xff339999L, 0xff009999L, 0xffff6699L, 0xffcc6699L, 0xff996699L, 0xff666699L, 0xff336699L,
		0xff006699L, 0xffff3399L, 0xffcc3399L, 0xff993399L, 0xff663399L, 0xff333399L, 0xff003399L, 0xffff0099L, 0xffcc0099L, 0xff990099L, 0xff660099L, 0xff330099L, 0xff000099L, 0xffffff66L, 0xffccff66L, 0xff99ff66L,
		0xff66ff66L, 0xff33ff66L, 0xff00ff66L, 0xffffcc66L, 0xffcccc66L, 0xff99cc66L, 0xff66cc66L, 0xff33cc66L, 0xff00cc66L, 0xffff9966L, 0xffcc9966L, 0xff999966L, 0xff669966L, 0xff339966L, 0xff009966L, 0xffff6666L,
		0xffcc6666L, 0xff996666L, 0xff666666L, 0xff336666L, 0xff006666L, 0xffff3366L, 0xffcc3366L, 0xff993366L, 0xff663366L, 0xff333366L, 0xff003366L, 0xffff0066L, 0xffcc0066L, 0xff990066L, 0xff660066L, 0xff330066L,
		0xff000066L, 0xffffff33L, 0xffccff33L, 0xff99ff33L, 0xff66ff33L, 0xff33ff33L, 0xff00ff33L, 0xffffcc33L, 0xffcccc33L, 0xff99cc33L, 0xff66cc33L, 0xff33cc33L, 0xff00cc33L, 0xffff9933L, 0xffcc9933L, 0xff999933L,
		0xff669933L, 0xff339933L, 0xff009933L, 0xffff6633L, 0xffcc6633L, 0xff996633L, 0xff666633L, 0xff336633L, 0xff006633L, 0xffff3333L, 0xffcc3333L, 0xff993333L, 0xff663333L, 0xff333333L, 0xff003333L, 0xffff0033L,
		0xffcc0033L, 0xff990033L, 0xff660033L, 0xff330033L, 0xff000033L, 0xffffff00L, 0xffccff00L, 0xff99ff00L, 0xff66ff00L, 0xff33ff00L, 0xff00ff00L, 0xffffcc00L, 0xffcccc00L, 0xff99cc00L, 0xff66cc00L, 0xff33cc00L,
		0xff00cc00L, 0xffff9900L, 0xffcc9900L, 0xff999900L, 0xff669900L, 0xff339900L, 0xff009900L, 0xffff6600L, 0xffcc6600L, 0xff996600L, 0xff666600L, 0xff336600L, 0xff006600L, 0xffff3300L, 0xffcc3300L, 0xff993300L,
		0xff663300L, 0xff333300L, 0xff003300L, 0xffff0000L, 0xffcc0000L, 0xff990000L, 0xff660000L, 0xff330000L, 0xff0000eeL, 0xff0000ddL, 0xff0000bbL, 0xff0000aaL, 0xff000088L, 0xff000077L, 0xff000055L, 0xff000044L,
		0xff000022L, 0xff000011L, 0xff00ee00L, 0xff00dd00L, 0xff00bb00L, 0xff00aa00L, 0xff008800L, 0xff007700L, 0xff005500L, 0xff004400L, 0xff002200L, 0xff001100L, 0xffee0000L, 0xffdd0000L, 0xffbb0000L, 0xffaa0000L,
		0xff880000L, 0xff770000L, 0xff550000L, 0xff440000L, 0xff220000L, 0xff110000L, 0xffeeeeeeL, 0xffddddddL, 0xffbbbbbbL, 0xffaaaaaaL, 0xff888888L, 0xff777777L, 0xff555555L, 0xff444444L, 0xff222222L, 0xff111111L
};


vec3 Voxel::get_color() {
	auto color_index = (int) (unsigned char) this->color;

	return ColorToVec(default_palette[color_index]);
}

Voxel::Voxel() {}

__int32_t ReadInt(FILE *fp) {
	__int32_t v = 0;
	fread(&v, 4, 1, fp);
	return v;
}

__int8_t ReadByte(FILE *fp) {
	__int8_t v = 0;
	fread(&v, 1, 1, fp);
	return v;
}


vec3 ColorToVec(const long color) {
	return vec3((color & 0xFF) / 255.0, ((color >> 8) & 0xFF) / 255.0, ((color >> 16) & 0xFF) / 255.0);
}

void ReadChunk(FILE *fp, chunk_t &chunk) {
	// read chunk
	chunk.id = ReadInt(fp);
	chunk.contentSize = ReadInt(fp);
	chunk.childrenSize = ReadInt(fp);

	// end of chunk : used for skipping the whole chunk
	chunk.end = ftell(fp) + chunk.contentSize + chunk.childrenSize;

	// print chunk info
	const char *c = (const char *) (&chunk.id);
	printf("[Log] MV_VoxelModel :: Chunk : %c%c%c%c : %d %d\n",
		   c[0], c[1], c[2], c[3],
		   chunk.contentSize, chunk.childrenSize
	);
}

int MV_ID(int a, int b, int c, int d) {
	return (a) | (b << 8) | (c << 16) | (d << 24);
}

//https://bitbucket.org/volumesoffun/cubiquity/src/9e1cb814c24e/Tools/ProcessVDB/Dependencies/MagicaVoxelModel.h?at=master

const int MV_VERSION = 150;
const int ID_VOX = MV_ID('V', 'O', 'X', ' ');
const int ID_MAIN = MV_ID('M', 'A', 'I', 'N');
const int ID_SIZE = MV_ID('S', 'I', 'Z', 'E');
const int ID_XYZI = MV_ID('X', 'Y', 'Z', 'I');
const int ID_RGBA = MV_ID('R', 'G', 'B', 'A');

std::vector<Voxel> ReadVox(std::string filename) {
	std::vector<Voxel> voxels;

	FILE *voxFile = fopen(filename.c_str(), "rb");
	if (voxFile) {
		// magic number
		int magic = ReadInt(voxFile);
		if (magic != ID_VOX) {
			Error("magic number does not match");
			return voxels;
		}

		// version
		int version = ReadInt(voxFile);
		if (version != MV_VERSION) {
			Error("version does not match");
			return voxels;
		}

		// main chunk
		chunk_t mainChunk;
		ReadChunk(voxFile, mainChunk);
		if (mainChunk.id != ID_MAIN) {
			Error("main chunk is not found");
			return voxels;
		}

		// read children chunks
		while (ftell(voxFile) < mainChunk.end) {
			// read chunk header
			chunk_t sub;
			ReadChunk(voxFile, sub);

			if (sub.id == ID_SIZE) {
				// size
				int sizex = ReadInt(voxFile);
				int sizey = ReadInt(voxFile);
				int sizez = ReadInt(voxFile);
				std::cout << "size : " << sizex << " " << sizey << " " << sizez << std::endl;
			} else if (sub.id == ID_XYZI) {
				// numVoxels
				int numVoxels = ReadInt(voxFile);
				if (numVoxels < 0) {
					Error("negative number of voxels");
					return voxels;
				}

				// voxels
				if (numVoxels > 0) {
					for (int i = 0; i < numVoxels; ++i) {
						int X = ReadByte(voxFile);

						int Y = ReadByte(voxFile);;

						int Z = ReadByte(voxFile);

						int color = ReadByte(voxFile);

						voxels.push_back(Voxel(X, Y, Z, color));
					}
				}
			}

			// skip unread bytes of current chunk or the whole unused chunk
			fseek(voxFile, sub.end, SEEK_SET);
		}
		std::cout << "Voxels: " << voxels.size() << std::endl;

	}
	return voxels;
}

static void
WriteVox(std::string filename, unsigned long X, unsigned long Y, unsigned long Z, std::vector<Voxel> voxels) {

	FILE *voxFile = fopen(filename.c_str(), "w");

	__int32_t version = 150;
	fwrite(&ID_VOX, 4, 1, voxFile);
	fwrite(&version, 4, 1, voxFile);

	__int32_t main_contentSize = 0;
	__int32_t main_childrenSize = 40 + voxels.size() * 4;
	fwrite(&ID_MAIN, 4, 1, voxFile);
	fwrite(&main_contentSize, 4, 1, voxFile);
	fwrite(&main_childrenSize, 4, 1, voxFile);

	__int32_t size_contentSize = 12;
	__int32_t size_childrenSize = 0;
	fwrite(&ID_SIZE, 4, 1, voxFile);
	fwrite(&size_contentSize, 4, 1, voxFile);
	fwrite(&size_childrenSize, 4, 1, voxFile);
	fwrite(&X, 4, 1, voxFile);
	fwrite(&Y, 4, 1, voxFile);
	fwrite(&Z, 4, 1, voxFile);

	__int32_t xyzi_contentSize = 4 + voxels.size() * 4;
	__int32_t xyzi_childrenSize = 0;
	__int32_t number_voxels = voxels.size();
	fwrite(&ID_XYZI, 4, 1, voxFile);
	fwrite(&xyzi_contentSize, 4, 1, voxFile);
	fwrite(&xyzi_childrenSize, 4, 1, voxFile);
	fwrite(&number_voxels, 4, 1, voxFile);

	for (Voxel v : voxels) {
		__int8_t x = v.x;
		__int8_t y = v.y;
		__int8_t z = v.z;
		__int8_t color = v.color;

		fwrite(&x, 1, 1, voxFile);
		fwrite(&y, 1, 1, voxFile);
		fwrite(&z, 1, 1, voxFile);
		fwrite(&color, 1, 1, voxFile);
	}
}