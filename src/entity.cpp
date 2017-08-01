/**
    player.cpp
    Purpose: Manage player logic and sprites

    @author: Zachary Vincze
    @version: 28/05/2017
*/

#include "entity.h"
#include "constants.h"

namespace {
    const int playerFrame = 0;

    const int verticalDrawOffset = 4;

    const int numWalkFrames = 4;
    const int numSidewayWalkFrames = 2;
    const int walkFps = 10;                // Change frames after certain amount of frames have passed
}

Entity::Entity(Graphics& graphics, int x, int y, const std::string& file_path) {
    _entity_rect.x = x * Constants::TILE_SIZE;
    _entity_rect.y = y * Constants::TILE_SIZE;
    _entity_rect.w = Constants::TILE_SIZE;
    _entity_rect.h = Constants::TILE_SIZE;
    
    _move_time = 0;
    _frames_to_cross_one_tile = 16;
    _speed = 16 / _frames_to_cross_one_tile;
    
    _current_tile = 0;

    _direction_facing = EAST;
    _motion_type = WALKING;

    // Load sprites
    // ----- STATIC SPRITES
    _sprites.push_back(new Sprite(graphics, file_path, 0, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    _sprites.push_back(new Sprite(graphics, file_path, 0, Constants::TILE_SIZE, Constants::TILE_SIZE, Constants::TILE_SIZE));
    _sprites.push_back(new Sprite(graphics, file_path, 0, Constants::TILE_SIZE * 2, Constants::TILE_SIZE, Constants::TILE_SIZE));
    _sprites.push_back(new Sprite(graphics, file_path, 0, Constants::TILE_SIZE * 3, Constants::TILE_SIZE, Constants::TILE_SIZE));

    // ----- ANIMATED SPRITES
    _sprites.push_back(new AnimatedSprite(graphics, file_path, 0, 0, Constants::TILE_SIZE, Constants::TILE_SIZE, walkFps, numWalkFrames));
    _sprites.push_back(new AnimatedSprite(graphics, file_path, 0, Constants::TILE_SIZE, Constants::TILE_SIZE, Constants::TILE_SIZE, walkFps, numWalkFrames));
    _sprites.push_back(new AnimatedSprite(graphics, file_path, 0, Constants::TILE_SIZE * 2, Constants::TILE_SIZE, Constants::TILE_SIZE, walkFps, numSidewayWalkFrames));
    _sprites.push_back(new AnimatedSprite(graphics, file_path, 0, Constants::TILE_SIZE * 3, Constants::TILE_SIZE, Constants::TILE_SIZE, walkFps, numSidewayWalkFrames));
}

Entity::~Entity() {
    for (std::vector<Sprite*>::iterator i = _sprites.begin(); i != _sprites.end(); i++) {
        delete (*i);
    }
}

void Entity::update(Map& map) {
    _sprites[getSpriteID()]->update();
    
    if (_move_time > 0) {
        --_move_time;
        switch (_direction_facing) {
            case NORTH: _entity_rect.y -= _speed; break;
            case SOUTH: _entity_rect.y += _speed; break;
            case WEST: _entity_rect.x -= _speed; break;
            case EAST: _entity_rect.x += _speed; break;
        }
    }
    
    if (touchesWall(map.getMap())) {
        switch (_direction_facing) {
            case NORTH: _entity_rect.y += _speed; break;
            case SOUTH: _entity_rect.y -= _speed; break;
            case WEST: _entity_rect.x += _speed; break;
            case EAST: _entity_rect.x -= _speed; break;
        }
    }
}

void Entity::draw(Graphics& graphics, SDL_Rect& camera) {
    // Drawing with the 4 pixel y offset from the original game
    _sprites[getSpriteID()]->draw(graphics, _entity_rect.x - camera.x, (_entity_rect.y - verticalDrawOffset) - camera.y);
}

int Entity::getSpriteID() {

    if (_move_time > 0) {
        _motion_type = WALKING;
    } else {
        _motion_type = STANDING;
    }

    if (_direction_facing == NORTH && _motion_type == STANDING) return 0;
    else if (_direction_facing == SOUTH && _motion_type == STANDING) return 1;
    else if (_direction_facing == WEST && _motion_type == STANDING) return 2;
    else if (_direction_facing == EAST && _motion_type == STANDING) return 3;
    else if (_direction_facing == NORTH && _motion_type == WALKING) return 4;
    else if (_direction_facing == SOUTH && _motion_type == WALKING) return 5;
    else if (_direction_facing == WEST && _motion_type == WALKING) return 6;
    else if (_direction_facing == EAST && _motion_type == WALKING) return 7;
    else return 0;
}

void Entity::move(int direction) {
    if (_move_time <= 0) {
        switch (direction) {
            case 0: _direction_facing = NORTH; break;
            case 1: _direction_facing = SOUTH; break;
            case 2: _direction_facing = WEST; break;
            case 3: _direction_facing = EAST; break;
        }
        _move_time = _frames_to_cross_one_tile;
    }
    
    else if (_move_time <= 1 && direction == _direction_facing) _move_time += _frames_to_cross_one_tile;
}

void Entity::turn(int direction) {
    if (_move_time <= 0) {
        switch (direction) {
            case 0: _direction_facing = NORTH; break;
            case 1: _direction_facing = SOUTH; break;
            case 2: _direction_facing = WEST; break;
            case 3: _direction_facing = EAST; break;
        }
    }
}

bool Entity::checkCollision(SDL_Rect a, SDL_Rect b) {
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;
    
    return true;
}

bool Entity::touchesWall(std::vector<Tile*> tiles) {
    for (int i = 0; i < tiles.size(); i++) {
        if (tiles[i]->getTileID() == 6) {
            if (checkCollision(_entity_rect, tiles[i]->getTileRect())) return true;
        }
    }
    
    return false;
}

void Entity::stop() {
    _move_time = 0;
}