import sympy as simp
from sympy import sin, pi
import pandas as pd
from labtool_ex2 import Project
from uncertainties import ufloat

if __name__ == "__main__":
    gm = {
        "Ue": r"V1",
        "Ua": r"V_a",
        "V": r"V",
        "offset": r"V_{offset}",
    }
    gv = {
        "Ua": r"\si{\volt}",
        "Ue": r"\si{\milli\volt}",
        "V": r"1000",
        "offset": r"\si{\volt}",
    }
    simp.var(list(gv))
    P = Project("OPV", global_variables=gv, global_mapping=gm, font=13)
    ax = P.figure.add_subplot()

    # Versuch 1
    P.load_data("./daten/aussteurer-aussteurer.csv")
    # print(P.data)
    P.print_ftable(
        P.data[["Ue", "dUe", "Ua", "dUa"]],
        name="messwertversuch1",
    )

    simp.var(list(gv))
    # Versuch 3
    # P.print_ftable(
    #     P.data[
    #         [
    #             "S",
    #             "dS",
    #             "Q1",
    #             "dQ1",
    #             "l",
    #             "dl",
    #             "P2",
    #             "dP2",
    #             "PV",
    #             "dPV",
    #             "n",
    #             "dn",
    #         ]
    #     ],
    #     name="wertversuch3_extra",
    #     split=True,
    # )
    P.plot_data(
        ax,
        "Ue",
        "Ua",
        label="Gemessene Daten",
        style="r",
        errors=True,
    )
    # P.plot_data(
    #     ax,
    #     "R",
    #     "P2",
    #     label="Nice",
    #     style="r",
    #     errors=True,
    # )
    simp.var(list(gv))
    # print(P.data)

    # P.plot_function(
    #     axes=ax,
    #     x="R",
    #     y="theoPR",
    #     expr=theoPR,
    #     label="theo. Leistungskurven",
    #     style="green",
    #     errors=True,
    # )
    P.data = P.data[P.data["Ue"].between(-200, 202)]

    Ua = V * Ue + offset

    P.plot_fit(
        axes=ax,
        x="Ue",
        y="Ua",
        eqn=Ua,
        style="b",
        label="Linearer Bereich",
        use_all_known=False,
        offset=[30, 10],
        guess={"V": 64, "offset": 0.01},
        bounds=[
            {"name": "V", "min": "0", "max": "100"},
            {"name": "offset", "min": "0", "max": "1"},
        ],
        add_fit_params=True,
        granularity=10000,
    )

    ax.set_title(f"OPV Charakteristische Aussteuerungskurve")
    P.ax_legend_all(loc=0)
    P.figure.savefig(f"./Output/{P.name}/opvmessung.png", dpi=400)
    P.figure.clear()
