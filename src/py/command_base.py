from abc import ABC, abstractmethod
from typing import Dict, Union, TypeVar
import helpers

T = TypeVar('T')


class CommandBase(ABC):

    def __init__(self: T, **cli_params: Dict[str, Union[bool, str]]) -> T:

        self.cli_params = cli_params
        self.raw_t = None
        self.raw_v = None
        self.normalized_t = None
        self.normalized_v = None

    @abstractmethod
    def acquire_data(self: T) -> None:
        pass

    def transform_data(self: T) -> None:
        self.normalized_t = helpers.normalize_time_series(helpers.results_to_ints(self.raw_t))
        self.normalized_v = helpers.normalize_voltage_series(helpers.results_to_ints(self.raw_v))

    def export_data(self: T) -> None:

        if self.cli_params['plot']:
            helpers.plot(self.normalized_t, self.normalized_v, self.cli_params['plot_path'])
        else:
            helpers.peek(self.normalized_t, self.normalized_v)

    def main(self: T) -> None:
        self.acquire_data()
        self.transform_data()
        self.export_data()
