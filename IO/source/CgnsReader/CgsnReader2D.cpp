#include <IO/CgnsReader2D.hpp>
 
CgnsReader2D::CgnsReader2D(const std::string& filePath) :
	CgnsReader(filePath) {
	this->gridData.dimension = this->cellDimension;
	this->gridData.thickness = 1.0;
	this->readNodes();
	this->readElements();
}

void CgnsReader2D::readNodes() {
	this->numberOfNodes = this->zoneSizes[0];
	cgsize_t one = 1;
	double coordinatesX[this->numberOfNodes];
	double coordinatesY[this->numberOfNodes];
	if (cg_coord_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, "CoordinateX", RealDouble, &one, &this->numberOfNodes, coordinatesX)) {
		std::cerr << "\nError reading CoordinateX" << std::endl;	
		cg_error_exit();
	}
	if (cg_coord_read(this->cgnsFile, this->cgnsBase, this->cgnsZone, "CoordinateY", RealDouble, &one, &this->numberOfNodes, coordinatesY)) {
		std::cerr << "\nError reading CoordinateY" << std::endl;	
		cg_error_exit();
	}

	this->gridData.coordinates.resize(this->numberOfNodes, std::vector<double>(3));
	for (int i = 0; i < this->numberOfNodes; i++) {
		this->gridData.coordinates[i][0] = coordinatesX[i]; 
		this->gridData.coordinates[i][1] = coordinatesY[i]; 
		this->gridData.coordinates[i][2] = 0.0; 
	}
}

void CgnsReader2D::readElements() {
	vector<int> bfe, wfe, aHee;
	int nRegions = 0;
	vector<string> rNames;
	if (gridData->dimension == 3) { //.................................................................................... THREE-DIMENSIONAL GRID CONNECTIVITY
		// Loop over the number of sections and read the element connectivities.
		// As CGNS starts the numbering at 1 the for-loop starts at 1 as well.
		vector< vector<int> > aTet, aHex, aPyr, aPri, aTri, aQua;
		vector<int> rTet, rHex, rPyr, rPri;
		for (int section = 1; section <= nSections; section++) {
			ElementType_t type;
			cgsize_t eBeg, eEnd; 
			int nBdry, parentFlag;
			vector<cgsize_t> conn;
			// Determines the element type and set the pointer for the connectivity accordingly.
			if (cg_section_read(cgfile, base, zone, section, buf, &type, &eBeg, &eEnd, &nBdry, &parentFlag) != CG_OK) {
				cerr << "\nThere was a problem when reading section" << section << endl;	cg_error_exit();
			}
			int eSize = eEnd - eBeg + 1;

			// Reads connectivity and builds auxiliar connectivity matrices for different element types
			switch(type) {
				case MIXED: {
					aTri.clear();
					aQua.clear();
					cgsize_t connSize;
					int npe;
					cg_ElementDataSize(cgfile, base, zone, section, &connSize);
					conn.resize(connSize);
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					int i = 0;
					for (int e = 0; e < eSize; ++e) {
						cg_npe(ElementType_t(conn[i]), &npe);
						vector<int> aux(npe);
						for (int k = 0; k < npe; ++k) aux[k] = conn[i+1+k]-1;
						switch(conn[i]) {
							case TETRA_4:
								aTet.push_back(aux);	rTet.push_back(nRegions);		break;
							case HEXA_8:
								aHex.push_back(aux);	rHex.push_back(nRegions);		break;
							case PYRA_5:
								aPyr.push_back(aux);	rPyr.push_back(nRegions);		break;
							case PENTA_6:
								aPri.push_back(aux);	rPri.push_back(nRegions);		break;
							case TRI_3:
								aTri.push_back(aux);	break;
							case QUAD_4:
								aQua.push_back(aux);	break;
						}
						i += npe + 1;
					}
					if (conn[0] == TRI_3 || conn[0] == QUAD_4) { // if section is actually a boundary
						BoundaryGridData newBoun;
						newBoun.triangleConnectivity.resize(aTri.size(), IntVector(3));
						newBoun.quadrangleConnectivity.resize(aQua.size(), IntVector(4));
						for (int e = 0; e < aTri.size(); ++e) 
							for (int k = 0; k < 3; ++k) 
								newBoun.triangleConnectivity[e][k] = aTri[e][k];
						for (int e = 0; e < aQua.size(); ++e) 
							for (int k = 0; k < 4; ++k) 
								newBoun.quadrangleConnectivity[e][k] = aQua[e][k];
						newBoun.name = buf;
						gridData->boundaries.push_back(newBoun);
						bfe.push_back(eBeg);
					}
					else {
						rNames.push_back(buf);
						nRegions++;
					}
					break; 
				}
				case TETRA_4: {
					int npe = 4;	conn.resize(npe*eSize);
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					vector<int> aux(npe);
					for (int e = 0; e < eSize; ++e) {
						for (int k = 0; k < npe; ++k) aux[k] = conn[e*npe+k]-1;
						aTet.push_back(aux);	rTet.push_back(nRegions);		
					}
					break; 
				}
				case HEXA_8: {
					int npe = 8;	conn.resize(npe*eSize);
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					vector<int> aux(npe);
					for (int e = 0; e < eSize; ++e) {
						for (int k = 0; k < npe; ++k) aux[k] = conn[e*npe+k]-1;
						aHex.push_back(aux);	rHex.push_back(nRegions);		
					}
					break; 
				}
				case PYRA_5: {
					int npe = 5;	conn.resize(npe*eSize);
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					vector<int> aux(npe);
					for (int e = 0; e < eSize; ++e) {
						for (int k = 0; k < npe; ++k) aux[k] = conn[e*npe+k]-1;
						aPyr.push_back(aux);	rPyr.push_back(nRegions);		
					}
					break; 
				}
				case PENTA_6: {
					int npe = 6;	conn.resize(npe*eSize);
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					vector<int> aux(npe);
					for (int e = 0; e < eSize; ++e) {
						for (int k = 0; k < npe; ++k) aux[k] = conn[e*npe+k]-1;
						aPri.push_back(aux);	rPri.push_back(nRegions);		
					}
					break; 
				}
				case QUAD_4: {
					int npe = 4;	conn.resize(npe*eSize);
					BoundaryGridData newBoun;
					newBoun.quadrangleConnectivity.resize(eSize, IntVector(npe));
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					for (int e = 0; e < eSize; ++e) 
						for (int k = 0; k < npe; ++k) 
							newBoun.quadrangleConnectivity[e][k] = conn[e*npe+k]-1;
					newBoun.name = buf;
					gridData->boundaries.push_back(newBoun);
					bfe.push_back(eBeg);
					break; 
				}
				case TRI_3: {
					int npe = 3;	conn.resize(npe*eSize);
					BoundaryGridData newBoun;
					newBoun.triangleConnectivity.resize(eSize, IntVector(npe));
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					for (int e = 0; e < eSize; ++e) 
						for (int k = 0; k < npe; ++k) 
							newBoun.triangleConnectivity[e][k] = conn[e*npe+k]-1;
					newBoun.name = buf;
					gridData->boundaries.push_back(newBoun);
					bfe.push_back(eBeg);
					break; 
				}
				case BAR_2: {
					int npe = 2;	conn.resize(npe*eSize);
					WellGridData newWell;
					newWell.lineConnectivity.resize(eSize, IntVector(npe));
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					for (int e = 0; e < eSize; ++e) 
						for (int k = 0; k < npe; ++k) 
							newWell.lineConnectivity[e][k] = conn[e*npe+k]-1;
					newWell.name = buf;
					gridData->wells.push_back(newWell);
					wfe.push_back(eBeg);
					break; 
				}
				case NODE:
					conn.resize(eSize);
					cg_elements_read(cgfile, base, zone, section, &conn[0], 0);
					for (int e = 0; e < eSize; ++e) aHee.push_back(conn[e]-1);
					break;
				default:
					cout << "\nUnsupported element encountered" << endl;	cg_error_exit();
					break;
			}
			if (type == TETRA_4 || type == HEXA_8 || type == PYRA_5 || type == PENTA_6) {
				rNames.push_back(buf);
				nRegions++;
			}
		}
}
