#include "laser.h"

// When I made this only God and I knew what it did
// Now only God knows

Color Laser::RED = Color::Red;
Color Laser::BLUE = Color::Blue;
Color Laser::GREEN = Color::Green;

Laser::Laser(Vector2f p1, Vector2f p2, Color& color) : m_direction(p2 - p1) {
	this->p1 = p1;
	this->p2 = p2;
	setPosition(p1);
	setThickness(4.f);
	setFillColor(color);
}

Vector2f& Laser::getP1() {
	return p1;
}

Vector2f& Laser::getP2() {
	return p2;
}

void Laser::setP2(Vector2f p) {
	p2 = p;
	m_direction = p2 - p1;
	Shape::update();
}

bool Laser::operator==(const Laser& other) {
	return p1 == other.p1 && p2 == other.p2 && getFillColor() == other.getFillColor();
}

void Laser::setThickness(float thickness) {
	m_thickness = thickness;
	Shape::update();
}

float Laser::getThickness() {
	return m_thickness;
}

float Laser::getDirection() {
	return atan2(m_direction.x, m_direction.y);
}

float Laser::getLength() const {
	return std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
}

size_t Laser::getPointCount() const {
	return 4;
}

Vector2f Laser::getPoint(size_t index) const {
	Vector2f unitDirection = m_direction / getLength();
	Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	Vector2f offset = (m_thickness / 2.f) * unitPerpendicular;

	switch (index)
	{
	default:
	case 0: return offset;
	case 1: return (m_direction + offset);
	case 2: return (m_direction - offset);
	case 3: return (-offset);
	}
}

LaserSystem::LaserSystem(Vector2f size, TextureFactory& textureFactory, TileGrid tileGrid) : GameSprite(), size(size), textureFactory(textureFactory), tileGrid(tileGrid) {
	setPosition(Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2));
	setTextureRect(IntRect(0, 0, size.x, size.y));
}

LaserSystem::LaserSystem() : GameSprite(), textureFactory(textureFactory), tileGrid(tileGrid) {}

Color& LaserSystem::getColorFromId(int colorId) {
	switch (colorId) {
	case 0:
		return Laser::RED;
	case 1:
		return Laser::BLUE;
	case 2:
		return Laser::GREEN;
	}
}

void LaserSystem::mouseAddConnectors(Vector2f& mousePos, vector<LaserObject>& connectedLaserObjects, LaserObjectManager& laserObjectManager, TileGrid& tileGrid) {
	if (connectedLaserObjects.size() == 0) {
		return;
	}
	LaserObject connector = LaserObject(textureFactory.getTexture("connector"), mousePos, true, 0);
	connector.setOrigin(8, 8);
	if (isMouseOver(mousePos) && !tileGrid.isSolidTile(connector)) {
		addConnector(mousePos, connector, connectedLaserObjects, tileGrid);
	}
}

void LaserSystem::addConnector(Vector2f pos, LaserObject connector, vector<LaserObject>& connectedLaserObjects, TileGrid& tileGrid) {
	int sourceColorId = -1;
	bool flag = false;
	for (LaserObject& connectedLaserObject : connectedLaserObjects) {
		if (!connectedLaserObject.isASource()) {
			continue;
		}
		if (connectedLaserObject.getWidth() != Tile::SIZE) {
			if (sourceColorId == -1) {
				sourceColorId = connectedLaserObject.getColorId();
				continue;
			}
			if (sourceColorId != connectedLaserObject.getColorId()) {
				flag = true;
				break;
			}
		}
		bool flag1 = false;
		for (int poweredPoint : poweredPoints) {
			if (hashPoint(connectedLaserObject.getPosition()) == poweredPoint) {
				flag1 = true;
				break;
			}
		}
		if (flag1) {
			if (sourceColorId == -1) {
				sourceColorId = connectedLaserObject.getColorId();
				continue;
			}
			if (sourceColorId != connectedLaserObject.getColorId()) {
				flag = true;
				break;
			}
		}
	}
	if (sourceColorId == -1 || flag) {
		return;
	}
	connectors.push_back(connector);
	connectors.back().setColorId(sourceColorId);
	for (LaserObject& connectedLaserObject : connectedLaserObjects) {
		if (connectedLaserObject.getWidth() != Tile::SIZE) {
			bool flag = false;
			for (int poweredPoint : poweredPoints) {
				if (hashPoint(connectedLaserObject.getPosition()) == poweredPoint) {
					flag = true;
					break;
				}
			}
			if (flag) {
				addLaser(Laser(connectedLaserObject.getPosition(), pos, getColorFromId(sourceColorId)), tileGrid);
			}
			else {
				addLaser(Laser(pos, connectedLaserObject.getPosition(), getColorFromId(sourceColorId)), tileGrid);
			}
		}
		else {
			if (connectedLaserObject.isASource()) {
				addLaser(Laser(connectedLaserObject.getPosition(), pos, getColorFromId(sourceColorId)), tileGrid);
			}
			else {
				addLaser(Laser(pos, connectedLaserObject.getPosition(), getColorFromId(sourceColorId)), tileGrid);
			}
		}
	}
}

void LaserSystem::mouseRemoveConnectors(Vector2f& mousePos, LaserObjectManager& laserObjectManager, TileGrid& tileGrid) {
	for (int i = connectors.size() - 1; i >= 0; i--) {
		if (connectors[i].isMouseOver(mousePos)) {
			removeConnector(i, tileGrid);
			return;
		}
	}
}

void LaserSystem::removeConnector(int idx, TileGrid& tileGrid) {
	removePoint(connectors[idx].getPosition(), tileGrid);
	connectors.erase(connectors.begin() + idx);
}

bool LaserSystem::isIntersecting(Laser& laser1, Laser& laser2) {
	Vector2f& p1 = laser1.getP1();
	Vector2f& p2 = laser1.getP2();
	Vector2f& p3 = laser2.getP1();
	Vector2f& p4 = laser2.getP2();
	if (p1 == p3 || p1 == p4 || p2 == p3 || p2 == p4) {
		return false;
	}
	bool flag1 = true;
	bool flag2 = true;
	for (int poweredPoint : poweredPoints) {
		if (hashPoint(laser1.getP1()) == poweredPoint) {
			flag1 = false;
		}
		if (hashPoint(laser2.getP1()) == poweredPoint) {
			flag2 = false;
		}
	}
	if (flag1 || flag2) {
		return false;
	}
	float d = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
	if (d == 0) {
		return false;
	}
	float pre = (p1.x * p2.y - p1.y * p2.x);
	float post = (p3.x * p4.y - p3.y * p4.x);
	float x = round((pre * (p3.x - p4.x) - (p1.x - p2.x) * post) / d *
		floatPrecision) /
		floatPrecision;
	float y = round((pre * (p3.y - p4.y) - (p1.y - p2.y) * post) / d *
		floatPrecision) /
		floatPrecision;
	if (x < min(p1.x, p2.x) - 1 / floatPrecision ||
		x > max(p1.x, p2.x + 1 / floatPrecision) ||
		x < min(p3.x, p4.x) - 1 / floatPrecision ||
		x > max(p3.x, p4.x + 1 / floatPrecision)) {
		return false;
	}
	if (y < min(p1.y, p2.y) - 1 / floatPrecision ||
		y > max(p1.y, p2.y + 1 / floatPrecision) ||
		y < min(p3.y, p4.y) - 1 / floatPrecision ||
		y > max(p3.y, p4.y + 1 / floatPrecision)) {
		return false;
	}
	return true;
}

Vector2f LaserSystem::getIntersectionPoint(Laser& laser1, Laser& laser2) {
	Vector2f& p1 = laser1.getP1();
	Vector2f& p2 = laser1.getP2();
	Vector2f& p3 = laser2.getP1();
	Vector2f& p4 = laser2.getP2();
	float d = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
	float pre = (p1.x * p2.y - p1.y * p2.x);
	float post = (p3.x * p4.y - p3.y * p4.x);
	float x = round((pre * (p3.x - p4.x) - (p1.x - p2.x) * post) / d *
		floatPrecision) /
		floatPrecision;
	float y = round((pre * (p3.y - p4.y) - (p1.y - p2.y) * post) / d *
		floatPrecision) /
		floatPrecision;
	return Vector2f(x, y);
}

bool LaserSystem::isIntersectingWithTile(Laser& laser, TileGrid& tileGrid) {
	float c0 = (laser.getP1().x - tileGrid.getX()) / Tile::SIZE;
	float r0 = (laser.getP1().y - tileGrid.getY()) / Tile::SIZE;
	float c1 = (laser.getP2().x - tileGrid.getX()) / Tile::SIZE;
	float r1 = (laser.getP2().y - tileGrid.getY()) / Tile::SIZE;

	float dc = (c1 - c0);
	float dr = (r1 - r0);
	for (int r = min(r0, r1); r < max(r0, r1); r++) {
		for (int c = min(c0, c1); c < max(c0, c1); c++) {
			if (tileGrid.getTile(r, c).isOpaque()) {
				if (c0 < c1 && r <= ((c - c0) / dc * dr + r0) && ((c - c0) / dc * dr + r0) <= r + 1) {
					return true;
				}
				else if (c0 > c1 && r <= ((c + 1 - c0) / dc * dr + r0) && ((c + 1 - c0) / dc * dr + r0) <= r + 1) {
					return true;
				}
				else if (r0 < r1 && c <= ((r - r0) / dr * dc + c0) && ((r - r0) / dr * dc + c0) <= c + 1) {
					return true;
				}
				else if (r0 > r1 && c <= ((r + 1 - r0) / dr * dc + c0) && ((r + 1 - r0) / dr * dc + c0) <= c + 1) {
					return true;
				}
			}
		}
	}
	return false;
}

Vector2f LaserSystem::getIntersectionWithTile(Laser& laser, TileGrid& tileGrid) {
	float c0 = (laser.getP1().x - tileGrid.getX()) / Tile::SIZE;
	float r0 = (laser.getP1().y - tileGrid.getY()) / Tile::SIZE;
	float c1 = (laser.getP2().x - tileGrid.getX()) / Tile::SIZE;
	float r1 = (laser.getP2().y - tileGrid.getY()) / Tile::SIZE;

	float dc = (c1 - c0);
	float dr = (r1 - r0);

	Vector2f res(1e6, 1e6);

	for (int r = min(r0, r1); r < max(r0, r1); r++) {
		for (int c = min(c0, c1); c < max(c0, c1); c++) {
			if (tileGrid.getTile(r, c).isOpaque()) {
				if (c0 < c1 && r <= ((c - c0) / dc * dr + r0) && ((c - c0) / dc * dr + r0) <= r + 1) {
					if (getDistance(laser.getP1(), Vector2f(Tile::SIZE * c, Tile::SIZE * ((c - c0) / dc * dr + r0)) + tileGrid.getPosition()) < getDistance(laser.getP1(), res)) {
						res = Vector2f(Tile::SIZE * c, Tile::SIZE * ((c - c0) / dc * dr + r0)) + tileGrid.getPosition();
					}
				}
				if (c0 > c1 && r <= ((c + 1 - c0) / dc * dr + r0) && ((c + 1 - c0) / dc * dr + r0) <= r + 1) {
					if (getDistance(laser.getP1(), Vector2f(Tile::SIZE * (c + 1), Tile::SIZE * ((c + 1 - c0) / dc * dr + r0)) + tileGrid.getPosition()) < getDistance(laser.getP1(), res)) {
						res = Vector2f(Tile::SIZE * (c + 1), Tile::SIZE * ((c + 1 - c0) / dc * dr + r0)) + tileGrid.getPosition();
					}
				}
				if (r0 < r1 && c <= ((r - r0) / dr * dc + c0) && ((r - r0) / dr * dc + c0) <= c + 1) {
					if (getDistance(laser.getP1(), Vector2f(Tile::SIZE * ((r - r0) / dr * dc + c0), Tile::SIZE * r) + tileGrid.getPosition()) < getDistance(laser.getP1(), res)) {
						res = Vector2f(Tile::SIZE * ((r - r0) / dr * dc + c0), Tile::SIZE * r) + tileGrid.getPosition();
					}
				}

				if (r0 > r1 && c <= ((r + 1 - r0) / dr * dc + c0) && ((r + 1 - r0) / dr * dc + c0) <= c + 1) {
					if (getDistance(laser.getP1(), Vector2f(Tile::SIZE * ((r + 1 - r0) / dr * dc + c0), Tile::SIZE * (r + 1)) + tileGrid.getPosition()) < getDistance(laser.getP1(), res)) {
						res = Vector2f(Tile::SIZE * ((r + 1 - r0) / dr * dc + c0), Tile::SIZE * (r + 1)) + tileGrid.getPosition();
					}
				}
			}
		}
	}
	return res;
}
void LaserSystem::updateLasers(Laser& laser1, Laser& laser2, TileGrid& tileGrid) {
	Vector2f oldIntersectionPoint;

	for (Laser& poweredLaser : poweredLasers) {
		if (laser2.getP1() == poweredLaser.getP1() && abs(laser2.getDirection() - poweredLaser.getDirection()) < 1e-6) {
			oldIntersectionPoint = laser1.getP2();
			break;
		}
	}

	Vector2f intersectionPoint = getIntersectionPoint(laser1, laser2);
	/*updateIntersectionPoint(laser2, oldIntersectionPoint, p, tileGrid);
	updateIntersectionPoint(laser1, oldIntersectionPoint, p, tileGrid);*/
	laser1.setP2(intersectionPoint);
	laser2.setP2(intersectionPoint);
	if (!oldIntersectionPoint.x && !oldIntersectionPoint.y) {
		return;
	}
	for (Laser laser : lasers) {
		if (laser.getP2() != oldIntersectionPoint) {
			continue;
		}
		for (Laser originalLaser : originalLasers) {
			if (laser.getP1() == originalLaser.getP1() && abs(laser.getDirection() - originalLaser.getDirection()) < 1e-6) {
				removeLaser(originalLaser, tileGrid);
				addLaser(originalLaser, tileGrid);
				break;
			}
		}
		break;
	}
}

void LaserSystem::updateIntersectionPoint(Laser& laser, Vector2f oldIntersectionPoint, Vector2f& intersectionPoint, TileGrid& tileGrid) {
	laser.setP2(intersectionPoint);
	if (!oldIntersectionPoint.x && !oldIntersectionPoint.y) {
		return;
	}
	for (Laser laser : lasers) {
		if (laser.getP2() != oldIntersectionPoint) {
			continue;
		}
		for (Laser originalLaser : originalLasers) {
			if (laser.getP1() == originalLaser.getP1() && abs(laser.getDirection() - originalLaser.getDirection()) < 1e-6) {
				removeLaser(originalLaser, tileGrid);
				addLaser(originalLaser, tileGrid);
				break;
			}
		}
		break;
	}
}

void LaserSystem::setPoweredLasers(LaserObjectManager& laserObjectManager) {
	unordered_map<int, vector<pair<Vector2f, Color>>> adjList;
	for (Laser& laser : originalLasers) {
		adjList[hashPoint(laser.getP1())].push_back({ laser.getP2(), laser.getFillColor() });
	}
	queue<pair<Vector2f, Color>> q;
	poweredPoints.clear();
	for (LaserObject& laserObject : laserObjectManager.getSourceLaserObjects()) {
		q.push({ laserObject.getPosition(), getColorFromId(laserObject.getColorId()) });
	}
	while (!q.empty()) {
		pair<Vector2f, Color> p = q.front();
		poweredPoints.insert(hashPoint(p.first));
		q.pop();
		for (pair<Vector2f, Color>& nextP : adjList[hashPoint(p.first)]) {
			if (poweredPoints.find(hashPoint(nextP.first)) == poweredPoints.end()) {
				if (find(lasers.begin(), lasers.end(), Laser(p.first, nextP.first, nextP.second)) != lasers.end()) {
					q.push({ nextP.first, nextP.second });
				}
			}
		}
	}
	poweredLasers.clear();
	for (Laser& orginalLaser : originalLasers) {
		if (poweredPoints.find(hashPoint(orginalLaser.getP1())) != poweredPoints.end()) {
			poweredLasers.push_back(orginalLaser);
		}
	}
}

void LaserSystem::addLaser(Laser newLaser, TileGrid& tileGrid) {
	originalLasers.push_back(newLaser);
	lasers.push_back(newLaser);
	vector<int> intersectionPoints0;
	vector<Vector2f> intersectionPoints1;
	for (int i = 0; i < lasers.size() - 1; i++) {
		if (isIntersecting(lasers[i], lasers.back())) {
			intersectionPoints0.push_back(i);
		}
	}
	if (isIntersectingWithTile(lasers.back(), tileGrid)) {
		intersectionPoints1.push_back(getIntersectionWithTile(lasers.back(), tileGrid));
	}
	if (intersectionPoints0.size() == 0 && intersectionPoints1.size() == 0) {
		return;
	}
	if (intersectionPoints0.size() == 1 && intersectionPoints1.size() == 0) {
		updateLasers(lasers[intersectionPoints0.back()], lasers.back(), tileGrid);
		return;
	}
	if (intersectionPoints0.size() == 0 && intersectionPoints1.size() == 1) {
		updateIntersectionPoint(lasers.back(), Vector2f(0, 0), intersectionPoints1.back(), tileGrid);
		return;
	}
	float minDist0 = FLT_MAX;
	int minDistLine0 = 0;
	for (int i : intersectionPoints0) {
		float distance = getDistance(lasers.back().getP1(),
			getIntersectionPoint(lasers[i], lasers.back()));
		if (minDist0 > distance) {
			minDist0 = distance;
			minDistLine0 = i;
		}
	}
	float minDist1 = FLT_MAX;
	Vector2f minDistLine1;
	for (Vector2f p : intersectionPoints1) {
		float distance = getDistance(lasers.back().getP1(), p);
		if (minDist1 > distance) {
			minDist1 = distance;
			minDistLine1 = p;
		}
	}
	if (minDist0 < minDist1) {
		updateLasers(lasers[minDistLine0], lasers.back(), tileGrid);
	}
	else {
		updateIntersectionPoint(lasers.back(), Vector2f(0, 0), minDistLine1, tileGrid);
	}
}

void LaserSystem::removePoint(Vector2f p, TileGrid& tileGrid) {
	// temp solution that might be permanent
	vector<Laser>orignalLasersClone(originalLasers);
	originalLasers.clear();
	lasers.clear();
	for (Laser laser : orignalLasersClone) {
		if (laser.getP1() != p && laser.getP2() != p) {
			addLaser(laser, tileGrid);
		}
	}
}

void LaserSystem::removeLaser(Laser laser0, TileGrid& tileGrid) {
	// temp solution that might be permanent
	vector<Laser>orignalLasersClone(originalLasers);
	originalLasers.clear();
	lasers.clear();
	for (Laser laser : orignalLasersClone) {
		if (laser.getP1() != laser0.getP1() || laser.getP2() != laser0.getP2()) {
			addLaser(laser, tileGrid);
		}
	}
}

vector<LaserObject>& LaserSystem::getConnectors() {
	return connectors;
}

int LaserSystem::hashPoint(Vector2f p) {
	return 7919 * p.x + p.y;
}

float LaserSystem::getDistance(Vector2f p1, Vector2f p2) {
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void LaserSystem::draw(RenderWindow& renderWindow, LaserObjectManager& laserObjectManager, TileGrid& tileGrid) {
	//laser.setPosition(laser.getPosition() + Vector2f((1664 - size.x) / 2 + 128, (1080 - size.y) / 2
	for (Door& door : tileGrid.getDoors()) {
		door.isOpen = false;
	}
	for (int r = 1; r < tileGrid.getN() - 1; r++) {
		for (int c = 1; c < tileGrid.getM(); c++) {
			if (!laserObjectManager.getLaserObject(r, c).isASource()) {
				bool flag = false;
				for (int poweredPoint : poweredPoints) {
					if (hashPoint(laserObjectManager.getLaserObject(r, c).getPosition()) == poweredPoint) {
						for (Laser& poweredLaser : poweredLasers) {
							if (hashPoint(poweredLaser.getP2()) == poweredPoint && poweredLaser.getFillColor() == getColorFromId(laserObjectManager.getLaserObject(r, c).getColorId())) {
								flag = true;
								break;

							}
						}
					}
				}
				if (tileGrid.isDoor(r + 1, c + 1)) {
					if (flag) {
						tileGrid.getDoor(r + 1, c + 1).isOpen = true;
					}
				}
				if (tileGrid.isDoor(r - 1, c + 1)) {
					if (flag) {
						tileGrid.getDoor(r - 1, c + 1).isOpen = true;
					}
				}
				if (tileGrid.isDoor(r + 1, c - 1)) {
					if (flag) {
						tileGrid.getDoor(r + 1, c - 1).isOpen = true;
					}
				}
				if (tileGrid.isDoor(r - 1, c - 1)) {
					if (flag) {
						tileGrid.getDoor(r - 1, c - 1).isOpen = true;
					}
				}
			}
		}
	}

	for (Laser orginalLaser : originalLasers) {
		orginalLaser.setFillColor(Color(84, 84, 84, 128));
		renderWindow.draw(orginalLaser);
	}

	setPoweredLasers(laserObjectManager);
	for (Laser& poweredLaser : poweredLasers) {
		removeLaser(poweredLaser, tileGrid);
		addLaser(poweredLaser, tileGrid);
	}

	GameSprite laserIntersection(textureFactory.getTexture("LaserIntersection"), Vector2f(0, 0));
	laserIntersection.setOrigin(laserIntersection.getWidth() / 2, laserIntersection.getHeight() / 2);
	for (Laser& poweredLaser : poweredLasers) {
		for (Laser& laser : lasers) {
			if (laser.getP1() == poweredLaser.getP1() && abs(laser.getDirection() - poweredLaser.getDirection()) < 1e-6) {
				renderWindow.draw(laser);
				laserIntersection.setPosition(laser.getP2());
				renderWindow.draw(laserIntersection);
				break;
			}
		}
	}

	for (GameSprite& connector : connectors) {
		renderWindow.draw(connector);
	}
}