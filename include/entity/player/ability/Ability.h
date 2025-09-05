//
// Created by Agamjeet Singh on 16/08/25.
//

#ifndef ABILITY_H
#define ABILITY_H

class Player;

class Ability {
public:
    virtual ~Ability() = default;

    explicit Ability(Player& player);

    [[nodiscard]] bool isPerforming() const {
        return performing;
    }

    [[nodiscard]] virtual bool canPerform() const = 0;
    virtual void perform() = 0;
    virtual void cancel() = 0;

protected:
    Player& player;
    bool performing = false;
    bool onCooldown = false;

    virtual void callDuring() = 0;
    virtual void startCooldown() = 0;
};



#endif //ABILITY_H
