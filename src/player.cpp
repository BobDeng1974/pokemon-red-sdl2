/**
    player.cpp
    Purpose: Manage player logic and sprites

    @author: Zachary Vincze
    @version: 28/05/2017
*/

#include "player.h"
#include "game.h"

namespace {
    const std::string spriteFilePath = "gfx/sprites/red.png";

    const float walkSpeed = 1;             // Pixles moved per frame
    const int playerFrame = 0;

    const int verticalDrawOffset = 4;

    const int numWalkFrames = 4;
    const int numSidewayWalkFrames = 2;
    const int walkFps = 10;                // Change frames after certain amount of frames have passed

    bool isWalking = false;
    bool atTarget = true;
}

Player::Player(Graphics& graphics, int x, int y) {
    mPlayerRect.x = x * 16;
    mPlayerRect.y = y * 16;
    mPlayerRect.w = 16;
    mPlayerRect.h = 16;
    mTempX = x * 16;
    mTempY = y * 16;
    mTargetX = x * 16;
    mTargetY = y * 16;
    mVelocityX = 0;
    mVelocityY = 0;
    mSprites.resize(8);

    mDirectionFacing = RIGHT;
    mMotionType = WALKING;

    // Load sprites
    // ----- STATIC SPRITES
    mSprites[0] = new Sprite(graphics, spriteFilePath, 0, 0, Game::kTileSize, Game::kTileSize);
    mSprites[1] = new Sprite(graphics, spriteFilePath, 0, Game::kTileSize, Game::kTileSize, Game::kTileSize);
    mSprites[2] = new Sprite(graphics, spriteFilePath, 0, Game::kTileSize * 2, Game::kTileSize, Game::kTileSize);
    mSprites[3] = new Sprite(graphics, spriteFilePath, 0, Game::kTileSize * 3, Game::kTileSize, Game::kTileSize);

    // ----- ANIMATED SPRITES
    mSprites[4] = new AnimatedSprite(graphics, spriteFilePath, 0, 0, Game::kTileSize, Game::kTileSize, walkFps, numWalkFrames);
    mSprites[5] = new AnimatedSprite(graphics, spriteFilePath, 0, Game::kTileSize, Game::kTileSize, Game::kTileSize, walkFps, numWalkFrames);
    mSprites[6] = new AnimatedSprite(graphics, spriteFilePath, 0, Game::kTileSize * 2, Game::kTileSize, Game::kTileSize, walkFps, numSidewayWalkFrames);
    mSprites[7] = new AnimatedSprite(graphics, spriteFilePath, 0, Game::kTileSize * 3, Game::kTileSize, Game::kTileSize, walkFps, numSidewayWalkFrames);
}

Player::~Player() {
    for (std::vector<Sprite*>::iterator i = mSprites.begin(); i != mSprites.end(); i++) {
        delete (*i);
    }
}

void Player::update() {
    mSprites[getSpriteID()]->update();

    // Start tile-based movement updates
    if (isWalking == true && atTarget == false) {
        mPlayerRect.x += mVelocityX;
        mPlayerRect.y += mVelocityY;
    }

    // Check if player has reached target, if so, make atTarget true
    if (mPlayerRect.x == mTargetX && mPlayerRect.y == mTargetY) atTarget = true;

    // Player has reached target, they must now stop
    if (atTarget == true && isWalking == true) stopMoving();

    printf("TARGET X: %i\n", mTargetX);
    printf("TARGET Y: %i\n", mTargetY);
}

void Player::draw(Graphics& graphics, SDL_Rect& camera) {
    // Drawing with the 4 pixel y offset from the original game
    mSprites[getSpriteID()]->draw(graphics, mPlayerRect.x - camera.x, (mPlayerRect.y - verticalDrawOffset) - camera.y);
}

int Player::getSpriteID() {

    if (mVelocityX != 0 || mVelocityY != 0) {
        mMotionType = WALKING;
    } else {
        mMotionType = STANDING;
    }

    if (mDirectionFacing == UP && mMotionType == STANDING)
        return 0;
    else if (mDirectionFacing == DOWN && mMotionType == STANDING)
        return 1;
    else if (mDirectionFacing == LEFT && mMotionType == STANDING)
        return 2;
    else if (mDirectionFacing == RIGHT && mMotionType == STANDING)
        return 3;
    else if (mDirectionFacing == UP && mMotionType == WALKING)
        return 4;
    else if (mDirectionFacing == DOWN && mMotionType == WALKING)
        return 5;
    else if (mDirectionFacing == LEFT && mMotionType == WALKING)
        return 6;
    else if (mDirectionFacing == RIGHT && mMotionType == WALKING)
        return 7;
    else
        return 0;
}

void Player::startMovingUp() {
    if (isWalking == false) {
        isWalking = true;
        atTarget = false;
        mTargetY = mPlayerRect.y - 16;
        mVelocityY = -walkSpeed;
        mDirectionFacing = UP;
    }
}

void Player::startMovingDown() {
    if (isWalking == false) {
        isWalking = true;
        atTarget = false;
        mTargetY = mPlayerRect.y + 16;
        mVelocityY = walkSpeed;
        mDirectionFacing = DOWN;
    }
}

void Player::startMovingLeft() {
    if (isWalking == false) {
        isWalking = true;
        atTarget = false;
        mTargetX = mPlayerRect.x - 16;
        mVelocityX = -walkSpeed;
        mDirectionFacing = LEFT;
    }
}

void Player::startMovingRight() {
    if (isWalking == false) {
        isWalking = true;
        atTarget = false;
        mTargetX = mPlayerRect.x + 16;
        mVelocityX = walkSpeed;
        mDirectionFacing = RIGHT;
    }
}

void Player::stopMoving() {
    isWalking = false;
    mVelocityX = 0;
    mVelocityY = 0;
}
