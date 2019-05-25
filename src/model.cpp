
#include "model.h"

bool Model::Run(int seed) {
	logT = std::log(T);
	logProb = std::vector<double>(T, 0);
	for (int t = 0; t < T; t++) logProb[t] = std::log(stationary[t]);
	Clear();
	std::mt19937 gen(seed);
	std::uniform_real_distribution<double> dis(0.0, 1.0);
	random = [&]() { return dis(gen); };

	int ite = 0;
	while (true) {
		ObserveStates result = Observe();

		if (result == True) return true;
		else if (result == False) return false;

		while (Propagate());
	}
}

Model::Model(std::string name, int FMX, int FMY, int FMZ, bool periodic, std::string groundName) {
this->FMX = FMX;
this->FMY = FMY;
this->FMZ = FMZ;
this->periodic = periodic;
this->ground = -1;
this->bounds = -1;

pugi::xml_document xdoc;
std::string filename = "../" + name + "/data.xml";
if (!xdoc.load_file(filename.c_str())) {
std::cout << "Error during opening of samples.xml";
}
pugi::xml_node xnode = xdoc.first_child();

voxelsize = xnode.attribute("voxelsize").as_int();

xnode = xnode.first_child();

std::vector<double> tempStationary;
std::vector<std::array<int, 8>> action;
std::map<std::string, int> firstOccurrence;

for (pugi::xml_node xtile: xnode.children()) {
std::string tilename = xtile.attribute("name").as_string();
std::string sym = xtile.attribute("symmetry").as_string();
std::function<int(int)> a, b;

// Default sym
int cardinality = 1;
a = [](int i) { return i; };
b = [](int i) { return i; };

if (!sym.compare("L")) {
cardinality = 4;
a = [](int i) { return (i + 1) % 4; };
b = [](int i) { return i % 2 == 0 ? i + 1 : i - 1; };
} else if (!sym.compare("T")) {
cardinality = 4;
a = [](int i) { return (i + 1) % 4; };
b = [](int i) { return i % 2 == 0 ? i : 4 - i; };
} else if (!sym.compare("I")) {
cardinality = 2;
a = [](int i) { return 1 - i; };
b = [](int i) { return i; };
}

T = action.size();
firstOccurrence[tilename] = T;

if (tilename.compare(groundName) == 0) {
ground = T;
}

if (tilename.compare("bounds") == 0) {
bounds = T;
}

for (int t = 0; t < cardinality; t++) {
std::array<int, 8> map;

map[0] = t;
map[1] = a(t);
map[2] = a(a(t));
map[3] = a(a(a(t)));
map[4] = b(t);
map[5] = b(a(t));
map[6] = b(a(a(t)));
map[7] = b(a(a(a(t))));

for (int s = 0; s < 8; ++s) {
map[s] += T;
}

action.push_back(map);
}
std::cout << "Opening: " << "../" << name << "/" << tilename << ".vox" << std::endl;
std::vector<Voxel> voxeltile = ReadVox("../" + name + "/" + tilename + ".vox");


tilenames.push_back(tilename + " 0");
voxeltiles.push_back(voxeltile);

for (int t = 1; t < cardinality; t++) {
tilenames.push_back("" + tilename + " " + std::to_string(t));

voxeltiles.push_back(rotateVoxels(voxeltiles.at(T + t - 1), voxelsize));
}

for (int t = 0; t < cardinality; t++) {

double w = xtile.attribute("weight").as_double();
if (w == 0)
w = 1;
tempStationary.push_back(w);
}

}

T = action.size();
stationary = tempStationary;

observed = Array3D<int>(FMX, FMY, FMZ, -1);
propagator = Array3D<bool>(6, T, T, false);
wave = Array4D<bool>(FMX, FMY, FMZ, T, false);
changes = Array3D<bool>(FMX, FMY, FMZ, false);


for (pugi::xml_node xneighbor : xnode.next_sibling().children()) {
std::string text = xneighbor.attribute("left").as_string();
std::vector<std::string> left;
boost::split(left, text, [](char c) { return c == ' '; });
text = xneighbor.attribute("right").as_string();
std::vector<std::string> right;
boost::split(right, text, [](char c) { return c == ' '; });

int L = action[firstOccurrence[left[0]]][left.size() == 1 ? 0 : std::atoi(left[1].c_str())];
int D = action[L][1];
int R = action[firstOccurrence[right[0]]][right.size() == 1 ? 0 : std::atoi(right[1].c_str())];
int U = action[R][1];

if (strcmp(xneighbor.name(), "horizontal") == 0) {
propagator.set(0, R, L, true);
propagator.set(0, action[R][6], action[L][6], true);
propagator.set(0, action[L][4], action[R][4], true);
propagator.set(0, action[L][2], action[R][2], true);

propagator.set(1, U, D, true);
propagator.set(1, action[D][6], action[U][6], true);
propagator.set(1, action[U][4], action[D][4], true);
propagator.set(1, action[D][2], action[U][2], true);
} else {
for (int g = 0; g < 8; g++) {
propagator.set(4, action[L][g], action[R][g], true);
}
}
}

for (int t2 = 0; t2 < T; t2++)
for (int t1 = 0; t1 < T; t1++) {
propagator.set(2, t2, t1, propagator.get(0, t1, t2));
propagator.set(3, t2, t1, propagator.get(1, t1, t2));
propagator.set(5, t2, t1, propagator.get(4, t1, t2));
}

std::cout << "Model created" << std::endl;

}


std::vector<Voxel> Model::rotateVoxels(std::vector<Voxel> array, size_t size) {
	std::vector<Voxel> result;
	for (Voxel voxel : array) {
		result.push_back(Voxel(size - 1 - voxel.y, voxel.x, voxel.z, voxel.color));
	}
	return result;
}

Model::ObserveStates Model::Observe() {
	double min = 1E+3, sum, mainSum, logSum, noise, entropy;
	int argminx = -1, argminy = -1, argminz = -1, amount;
	std::vector<bool> w;

	for (int x = 0; x < FMX; x++)
		for (int y = 0; y < FMY; y++)
			for (int z = 0; z < FMZ; z++) {
				amount = 0;
				sum = 0;

				for (int t = 0; t < T; t++)
					if (wave.get(x, y, z, t)) {

						amount += 1;
						sum += stationary[t];
					}
				if (sum == 0)
					return False;

				noise = 1E-6 * random();

				if (amount == 1)
					entropy = 0;
				else if (amount == T)
					entropy = logT;
				else {
					mainSum = 0;
					logSum = std::log(sum);
					for (int t = 0; t < T; t++)
						if (wave.get(x, y, z, t))
							mainSum += stationary[t] * logProb[t];
					entropy = logSum - mainSum / sum;
				}
				if (entropy > 0 && entropy + noise < min) {
					min = entropy + noise;
					argminx = x;
					argminy = y;
					argminz = z;
				}
			}

	if (argminx == -1 && argminy == -1 && argminz == -1) {
		for (int x = 0; x < FMX; x++)
			for (int y = 0; y < FMY; y++)
				for (int z = 0; z < FMZ; z++)
					for (int t = 0; t < T; t++)
						if (wave.get(x, y, z, t)) {
							observed.set(x, y, z, t);
							break;
						}

		return True;
	}

	std::vector<double> distribution(T, 0);
	for (int t = 0; t < T; t++)
		distribution[t] = wave.get(argminx, argminy, argminz, t) ? stationary[t] : 0;
	size_t r = rotateVector(std::move(distribution), random());
	for (int t = 0; t < T; t++)
		wave.set(argminx, argminy, argminz, t, t == r);
	changes.set(argminx, argminy, argminz, true);

	return Unfinished;
}

bool Model::Propagate() {
	bool change = false, b;
	for (int x2 = 0; x2 < FMX; x2++)
		for (int y2 = 0; y2 < FMY; y2++)
			for (int z2 = 0; z2 < FMZ; z2++)
				for (int d = 0; d < 6; d++) {
					int x1 = x2, y1 = y2, z1 = z2;
					if (d == 0) {
						if (x2 == 0) {
							if (!periodic) continue;
							else x1 = FMX - 1;
						} else x1 = x2 - 1;
					} else if (d == 1) {
						if (y2 == FMY - 1) {
							if (!periodic) continue;
							else y1 = 0;
						} else y1 = y2 + 1;
					} else if (d == 2) {
						if (x2 == FMX - 1) {
							if (!periodic) continue;
							else x1 = 0;
						} else x1 = x2 + 1;
					} else if (d == 3) {
						if (y2 == 0) {
							if (!periodic) continue;
							else y1 = FMY - 1;
						} else y1 = y2 - 1;
					} else if (d == 4) {
						if (z2 == FMZ - 1) {
							if (!periodic) continue;
							else z1 = 0;
						} else z1 = z2 + 1;
					} else {
						if (z2 == 0) {
							if (!periodic) continue;
							else z1 = FMZ - 1;
						} else z1 = z2 - 1;
					}

					if (!changes.get(x1, y1, z1)) continue;

					for (int t2 = 0; t2 < T; t2++)
						if (wave.get(x2, y2, z2, t2)) {
							b = false;

							for (int t1 = 0; t1 < T && !b; t1++)
								if (wave.get(x1, y1, z1, t1))
									b = propagator.get(d, t2, t1);

							if (!b) {
								wave.set(x2, y2, z2, t2, false);
								changes.set(x2, y2, z2, true);
								change = true;
							}
						}
				}

	return change;
}

void Model::Clear() {
	for (int x = 0; x < FMX; x++)
		for (int y = 0; y < FMY; y++)
			for (int z = 0; z < FMZ; z++) {
				for (int t = 0; t < T; t++)
					wave.set(x, y, z, t, true);
				changes.set(x, y, z, false);
			}

	if (bounds >= 0) {
		for (int y = 0; y < FMY; y++)
			for (int z = 0; z < FMZ - 2; z++) {
				for (int t = 0; t < T; t++)
					if (t != bounds) {
						wave.set(FMX - 1, y, z, t, false);
						wave.set(0, y, z, t, false);
					}


				changes.set(FMX - 1, y, z, true);
				changes.set(0, y, z, true);

				for (int x = 1; x < FMX - 1; x++) {
					if (y > 0 && y < FMY - 1) {
						wave.set(x, y, z, bounds, false);
						changes.set(x, y, z, true);
					}
				}
			}
		for (int x = 0; x < FMX; x++)
			for (int z = 0; z < FMZ - 2; z++) {
				for (int t = 0; t < T; t++)
					if (t != bounds) {
						wave.set(x, FMY - 1, z, t, false);
						wave.set(x, 0, z, t, false);
					}

				changes.set(x, FMY - 1, z, true);
				changes.set(x, 0, z, true);

				for (int y = 1; y < FMY - 1; y++) {
					if (x > 0 && x < FMX - 1) {
						wave.set(x, y, z, bounds, false);
						changes.set(x, y, z, true);
					}
				}
			}
	}
	if (ground >= 0) {
		for (int x = 0; x < FMX; x++)
			for (int y = 0; y < FMY; y++) {
				for (int t = 0; t < T; t++)
					if (t != ground)
						wave.set(x, y, FMZ - 1, t, false);
				changes.set(x, y, FMZ - 1, true);

				for (int z = 0; z < FMZ - 1; z++) {
					wave.set(x, y, z, ground, false);
					changes.set(x, y, z, true);
				}
			}
	}
}

void Model::saveVoxelOutput(std::string filename)
{
	std::vector<Voxel> result;
	for (int8_t x = 0; x < FMX; x++) for (int8_t y = 0; y < FMY; y++) for (int8_t z = 0; z < FMZ; z++)
			{

				int index = observed.get(x, FMY - y - 1, FMZ - z - 1);

				if(index >= 0) {
					std::vector<Voxel> voxeltile = voxeltiles[index];
					for (Voxel v : voxeltile)
						result.push_back(Voxel((v.x + x * voxelsize), (v.y + y * voxelsize), (v.z + z * voxelsize),
											   v.color));
				}
			}

	WriteVox(filename, FMX * voxelsize, FMY * voxelsize, FMZ * voxelsize, result);
}